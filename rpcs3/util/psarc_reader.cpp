#include "stdafx.h"
#include "psarc_reader.h"
#include <zlib.h>

LOG_CHANNEL(psarc_log, "PSARC");

template<typename T>
inline T read_le(const fs::file& f)
{
	T ret{};
	f.read(&ret, sizeof(ret));
	return ret;
}

template<typename T>
inline T read_be(const fs::file& f)
{
	be_t<T> ret;
	f.read(&ret, sizeof(ret));
	return ret;
}

inline u32 read_be_24(const fs::file& f)
{
	std::array<u8, 3> ret{};
	f.read(ret.data(), sizeof(ret));
	return (ret[0] << 16) | (ret[1] << 8) | ret[2];
}

inline u64 read_be_40(const fs::file& f)
{
	std::array<u8, 5> ret{};
	f.read(ret.data(), sizeof(ret));

	u64 res = ret[4] | (0x100 * ret[3]) | (ret[2] * 0x10000) | (ret[1] * 0x1000000) | (ret[0] * 0x100000000);
	if (ret[0] == 0)
		res %= 0xffffffff00000000;
	else
		res %= 0xfffffffe00000000;
	return res;
}

bool psarc_reader::extract_entry(usz id, const std::vector<u32>& block_sizes, std::string path)
{
	ensure(id < m_entries.size());
	ensure(!path.empty());

	fs::file stream{ path };

	if (!stream)
	{
		psarc_log.error("Failed to open stream file '%s' (%s)", path, fs::g_tls_error);
		return false;
	}

	const u64& uncompressed_size = m_entries[id].uncompressed_size;

	// Decompress file
	if (uncompressed_size != 0)
	{
		m_file.seek(m_entries[id].file_offset);

		// Decompress all blocks of this file until we reached the expected file size
		for (u32 i = m_entries[id].block_offset; stream.pos() < uncompressed_size; i++)
		{
			ensure(i < block_sizes.size());

			if (block_sizes[i] == 0)
			{
				// Assume default block size and no compression
				m_file.read(m_buffer.data(), m_header.block_size);
				stream.write(m_buffer.data(), m_header.block_size);
				continue;
			}

			// Read the compression level
			const u16 compression_level = read_be<u16>(m_file);

			// Read the (compressed) block
			m_file.seek(m_entries[id].file_offset);
			m_file.read(m_buffer, block_sizes[i]);

			const bool is_zipped = compression_level == 0x78DA; // TODO: see compression levels of gzip
			if (is_zipped)
			{
				// Determine destination size (in multiples of block size)
				const u32 block_index = i - m_entries[id].block_offset;
				const u32 bytes_read = block_index * m_header.block_size;
				const u64 bytes_left = uncompressed_size > bytes_read ? uncompressed_size - bytes_read : 0;

				unsigned long dst_size = ::narrow<unsigned long>(std::min<u64>(m_header.block_size, bytes_left));

				// Decompress the block
				if (const int err = uncompress(m_decompressed.data(), &dst_size, m_buffer.data(), block_sizes[i]); err != Z_OK)
				{
					psarc_log.error("Failed to uncompress (err=%d)", err);
					return false;
				}

				// Write decompressed block data to file
				stream.write(m_decompressed.data(), dst_size);
			}
			else
			{
				// Write raw uncompressed block data to file
				stream.write(m_buffer.data(), block_sizes[i]);
			}
		}
	}

	// Read file names from unpacked manifest
	if (id == 0)
	{
		stream.seek(0);

		m_entries[0].name = "NamesBlock.bin";

		for (usz i = 1; i < m_entries.size(); i++)
		{
			psarc_toc& entry = m_entries.at(i);
			entry.name.clear();

			char byte{};
			while (stream.read(&byte, 1) == 1 && byte != '\n' && stream.pos() < stream.size())
			{
				entry.name += byte;
			}
		}
	}
	else if (stream.size() != uncompressed_size)
	{
		psarc_log.error("Unexpected file size: '%s' (size=0x%x, expected=0x%x)", stream.pos(), uncompressed_size);
		return false;
	}

	return true;
}

bool psarc_reader::read(const std::string& path, bool info_only)
{
	psarc_log.notice("Reading file: '%s' (info_only=%d)", path, info_only);

	m_header = {};

	if (!m_file.open(path, fs::read))
	{
		psarc_log.error("Failed to open file '%s' (%s)", path, fs::g_tls_error);
		return false;
	}

	if (m_file.size() < sizeof(psarc_header))
	{
		psarc_log.error("Failed to read file '%s' (size=%d, min=%d)", path, m_file.size(), sizeof(psarc_header));
		return false;
	}

	const std::string filename = path.substr(path.find_last_of(fs::delim) + 1);

	// Read header
	m_header.magic = read_le<u32>(m_file);
	m_header.version_major = read_be<u16>(m_file);
	m_header.version_minor = read_be<u16>(m_file);
	m_header.compression_type = read_le<u32>(m_file);
	m_header.toc_length = read_be<u32>(m_file);
	m_header.toc_entry_size = read_be<u32>(m_file);
	m_header.toc_entries = read_be<u32>(m_file);
	m_header.block_size = read_be<u32>(m_file);
	m_header.archive_flags = read_be<u32>(m_file); // TODO: Use

	// Print header
	psarc_log.notice("File: '%s' (magic=0x%x, version=%d.%d, compression_type=0x%x, toc_length=0x%x, toc_entry_size=0x%x, toc_entries=%d, block_size=0x%x)",
		m_header.magic, m_header.version_major, m_header.version_minor, m_header.compression_type, m_header.toc_length, m_header.toc_entry_size, m_header.toc_entries, m_header.block_size);

	constexpr u32 psarc_magic = "?PSAR"_u32;

	if (m_header.magic != psarc_magic)
	{
		psarc_log.error("File does not start with psarc magic: '%s' (magic=0x%x, expected=0x%x)", path, m_header.magic, psarc_magic);
		return false;
	}

	constexpr u32 zlib_magic = "?zlib"_u32;
	constexpr u32 lzma_magic = "?lzma"_u32; // TODO: Handle

	if (m_header.compression_type != zlib_magic)
	{
		psarc_log.error("File compression type is not zlib magic: '%s' (magic=0x%x, expected=0x%x)", path, m_header.compression_type, zlib_magic);
		return false;
	}

	if (m_header.toc_length == 0)
	{
		psarc_log.error("File toc length is 0: '%s'", path);
		return false;
	}

	if (m_header.toc_entries == 0)
	{
		psarc_log.error("File toc entries count is 0: '%s'", path);
		return false;
	}

	if (m_header.block_size == 0)
	{
		psarc_log.error("File block size is 0: '%s'", path);
		return false;
	}

	if (m_header.toc_entry_size != 30) // See sum of bytes read in the loop below
	{
		// Let's just log this for now
		psarc_log.warning("File toc entry size has an unexpected value: '%s' (size=%d, expected=30)", path, m_header.toc_entry_size);
	}

	// Read Table of Contents
	m_file.seek(sizeof(psarc_header));
	m_entries.resize(m_header.toc_entries);

	for (u32 i = 0; i < m_header.toc_entries; i++)
	{
		psarc_toc& entry = m_entries.at(i);
		entry.id = i;
		entry.name_digest = read_be<u128>(m_file);
		entry.block_offset = read_be<u32>(m_file);
		entry.uncompressed_size = read_be_40(m_file);
		entry.file_offset = read_be_40(m_file);
		entry.name.clear();
	}

	// Determine bit width of block size table
	u8 type = 1;
	for (u32 i = 256; i < m_header.block_size; i *= 256)
	{
		type++;
	}

	// Read block size table and calculate the necessary buffer size.
	u32 buffersize = m_header.block_size;
	std::vector<u32> block_sizes((m_header.toc_length - ::narrow<u32>(sizeof(psarc_header))) / type, 0);

	for (u32& block_size : block_sizes)
	{
		switch (type)
		{
		case 1:
			block_size = read_le<u8>(m_file);
			break;
		case 2:
			block_size = read_be<u16>(m_file);
			break;
		case 3:
			block_size = read_be_24(m_file);
			break;
		case 4:
			block_size = read_be<u32>(m_file);
			break;
		default:
			fmt::throw_exception("psarc_reader: Unexpected type");
		}

		buffersize = std::max(buffersize, block_size);
	}

	m_buffer.resize(buffersize);
	m_decompressed.resize(buffersize);

	const std::string basedir = filename.ends_with(".psarc") ? fs::get_parent_dir(filename) : (filename + "_data");

	// Unpack manifest to a temporary file
	if (!extract_entry(0, block_sizes, "tmp/psarc.temp"))
	{
		// Note: Error logged in extract_entry
		return false;
	}

	// Create list of files if requested
	if (info_only)
	{
		const std::string outlist = filename + ".txt";
		fs::file list = fs::file(outlist, fs::rewrite);

		if (!list)
		{
			psarc_log.error("Failed to open list file '%s' (%s)", outlist, fs::g_tls_error);
			return false;
		}

		for (u32 i = 1; i < m_entries.size(); i++)
		{
			const psarc_toc& entry = m_entries.at(i);

			std::string msg;

			if (entry.uncompressed_size < 0x400L)
			{
				fmt::append(msg, "%d %d b %s\n", i, entry.uncompressed_size, entry.name);
			}
			else if (entry.uncompressed_size < 0x100000L)
			{
				fmt::append(msg, "%d %1.2f Kb %s\n", i, entry.uncompressed_size / static_cast<double>(0x400L), entry.name);
			}
			else if (entry.uncompressed_size < 0x40000000L)
			{
				fmt::append(msg, "%d %1.2f Mb %s\n", i, entry.uncompressed_size / static_cast<double>(0x100000L), entry.name);
			}
			else
			{
				fmt::append(msg, "%d %1.2f Gb %s\n", i, entry.uncompressed_size / static_cast<double>(0x40000000L), entry.name);
			}

			list.write(msg);
		}

		return true;
	}

	if (!fs::create_path(basedir))
	{
		psarc_log.error("Failed to create path '%s' (%s)", basedir, fs::g_tls_error);
		return false;
	}

	// Unpack rest of the files
	for (usz i = 1; i < m_entries.size(); i++)
	{
		const psarc_toc& entry = m_entries.at(i);
		const std::string path = basedir + "/" + entry.name;
		const std::string dir = fs::get_parent_dir(path);

		psarc_log.notice("psarc_reader: entry %i: '%s'", entry.id, entry.name);

		if (!fs::create_path(dir))
		{
			psarc_log.error("Failed to create path '%s' (%s)", dir, fs::g_tls_error);
			return false;
		}

		if (!extract_entry(i, block_sizes, path))
		{
			// Note: Error logged in extract_entry
			return false;
		}
	}

	return true;
}

bool psarc_reader::read(const std::string& path)
{
	return read(path, false);
}

bool psarc_reader::read_header(const std::string& path)
{
	return read(path, true);
}
