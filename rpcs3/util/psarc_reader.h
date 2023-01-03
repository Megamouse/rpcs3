#pragma once

struct psarc_reader
{
	enum class psarc_path_flag : u32
	{
		relative   = 0x0,
		ignorecase = 0x1,
		absolute   = 0x2,
	};

	struct psarc_header
	{
		u32 magic = 0;
		u16 version_major = 0;
		u16 version_minor = 0;
		u32 compression_type = 0;
		u32 toc_length = 0;
		u32 toc_entry_size = 0;
		u32 toc_entries = 0;
		u32 block_size = 0;
		u32 archive_flags = 0;
	};
	static_assert(sizeof(psarc_header) == 32UL);

	struct psarc_toc
	{
		u128 name_digest = 0;
		u32 block_offset = 0;
		u64 uncompressed_size = 0;
		u64 file_offset = 0;

		// Meta members
		u32 id = 0;
		std::string name;
	};

public:
	bool read(const std::string& path);
	bool read_header(const std::string& path);

private:
	bool extract_entry(usz id, const std::vector<u32>& block_sizes, std::string path);
	bool read(const std::string& path, bool header);

	fs::file m_file{};
	psarc_header m_header{};
	std::vector<psarc_toc> m_entries;
	std::vector<u8> m_buffer;
	std::vector<u8> m_decompressed;
};
