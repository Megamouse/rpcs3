#include "stdafx.h"
#include "Emu/Cell/PPUModule.h"
#include "Emu/IdManager.h"
#include "Emu/System.h"
#include "cellPngEnc.h"
#include "png.h"

#include <deque>

LOG_CHANNEL(cellPngEnc);

template <>
void fmt_class_string<CellPngEncError>::format(std::string& out, u64 arg)
{
	format_enum(out, arg, [](CellPngEncError value)
	{
		switch (value)
		{
		STR_CASE(CELL_PNGENC_ERROR_ARG);
		STR_CASE(CELL_PNGENC_ERROR_SEQ);
		STR_CASE(CELL_PNGENC_ERROR_BUSY);
		STR_CASE(CELL_PNGENC_ERROR_EMPTY);
		STR_CASE(CELL_PNGENC_ERROR_RESET);
		STR_CASE(CELL_PNGENC_ERROR_FATAL);
		STR_CASE(CELL_PNGENC_ERROR_STREAM_ABORT);
		STR_CASE(CELL_PNGENC_ERROR_STREAM_SKIP);
		STR_CASE(CELL_PNGENC_ERROR_STREAM_OVERFLOW);
		STR_CASE(CELL_PNGENC_ERROR_STREAM_FILE_OPEN);
		}

		return unknown;
	});
}

static void PNGCBAPI log_warning([[maybe_unused]] png_structp png_ptr, png_const_charp message)
{
	cellPngEnc.warning("png warning: %s", message);
}

static void PNGCBAPI log_error([[maybe_unused]] png_structp png_ptr, png_const_charp message)
{
	fmt::throw_exception("png error: %s", message);
}

struct png_encoder
{
	void operator()()
	{
		struct scoped_png_ptrs
		{
			png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
			png_infop info_ptr    = png_create_info_struct(write_ptr);

			~scoped_png_ptrs()
			{
				png_free_data(write_ptr, info_ptr, PNG_FREE_ALL, -1);
				png_destroy_write_struct(&write_ptr, &info_ptr);
			}
		};

		while (true)
		{
			if (thread_ctrl::state() == thread_state::aborting)
			{
				break;
			}

			if (Emu.IsPaused() || handles.empty())
			{
				thread_ctrl::wait_for(10000);
				continue;
			}

			std::lock_guard lock(mutex);

			bool encoded_something = false;

			// Decode one png of each handle
			for (auto& [handle, data] : handles)
			{
				for (png_encoder::png_stream& stream : data.encode_queue)
				{
					if (stream.is_ready || !stream.is_encoding || stream.state != CELL_OK)
					{
						continue;
					}

					cellPngEnc.todo("encoding png...");

					ensure(stream.picture.pictureAddr);
					u8* src = static_cast<u8*>(stream.picture.pictureAddr.get_ptr());

					// Create row pointers for libpng
					std::vector<u8*> rows(stream.picture.height);
					for (u32 y = 0; y < stream.picture.height; y++)
					{
						rows[y] = src + y * stream.picture.pitchWidth;
					}

					std::vector<u8> encoded_png;

					int colorSpace = PNG_COLOR_TYPE_GRAY;

					switch (stream.picture.colorSpace)
					{
					case CELL_PNGENC_COLOR_SPACE_GRAYSCALE: colorSpace = PNG_COLOR_TYPE_GRAY; break;
					case CELL_PNGENC_COLOR_SPACE_PALETTE: colorSpace = PNG_COLOR_TYPE_PALETTE; break;
					case CELL_PNGENC_COLOR_SPACE_RGB: colorSpace = PNG_COLOR_TYPE_RGB; break;
					case CELL_PNGENC_COLOR_SPACE_GRAYSCALE_ALPHA: colorSpace = PNG_COLOR_TYPE_GRAY_ALPHA; break;
					case CELL_PNGENC_COLOR_SPACE_RGBA: colorSpace = PNG_COLOR_TYPE_RGBA; break;
					case CELL_PNGENC_COLOR_SPACE_ARGB: colorSpace = PNG_COLOR_TYPE_RGBA; break; // TODO
					default: fmt::throw_exception("Unknown colorSpace %d", stream.picture.colorSpace);
					}

					// TODO: packedPixel
					// TODO: compressionLevel
					// TODO: filterType

					// Encode PNG
					const scoped_png_ptrs ptrs;
					png_set_error_fn(ptrs.write_ptr, nullptr, log_error, log_warning);
					png_set_IHDR(ptrs.write_ptr, ptrs.info_ptr, stream.picture.width, stream.picture.height, stream.picture.bitDepth, colorSpace, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
					png_set_rows(ptrs.write_ptr, ptrs.info_ptr, &rows[0]);
					png_set_write_fn(ptrs.write_ptr, &encoded_png, [](png_structp png_ptr, png_bytep data, png_size_t length)
					{
						std::vector<u8>* p = static_cast<std::vector<u8>*>(png_get_io_ptr(png_ptr));
						p->insert(p->end(), data, data + length);
					}, nullptr);
					png_write_png(ptrs.write_ptr, ptrs.info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);

					// Write file
					if (stream.location == CELL_PNGENC_LOCATION_FILE)
					{
						if (fs::file png_file(stream.streamFileName, fs::open_mode::create + fs::open_mode::write + fs::open_mode::excl); !png_file)
						{
							cellPngEnc.error("Failed to save png \"%s\" : %s", stream.streamFileName, fs::g_tls_error);
							stream.state = CELL_PNGENC_ERROR_STREAM_FILE_OPEN;
						}
						else if (u64 bytes_written = png_file.write(encoded_png.data(), encoded_png.size()); bytes_written < encoded_png.size())
						{
							cellPngEnc.error("Error saving png \"%s\" : bytes_written=%d, expected=%d", stream.streamFileName, bytes_written, encoded_png.size());
						}
					}
					else if (!stream.streamAddr)
					{
						cellPngEnc.error("Error writing png to buffer. buffer is null");
						stream.state = CELL_PNGENC_ERROR_STREAM_OVERFLOW;
					}
					else if (::size32(encoded_png) > stream.limitSize)
					{
						cellPngEnc.error("Error writing png to buffer. size=%d, limitSize=%d", encoded_png.size(), stream.limitSize);
						stream.state = CELL_PNGENC_ERROR_STREAM_OVERFLOW;
					}
					else
					{
						std::memcpy(stream.streamAddr.get_ptr(), encoded_png.data(), encoded_png.size());
					}

					stream.streamSize = ::size32(encoded_png);
					stream.processedLine = stream.picture.height;
					stream.is_encoding = false;
					stream.is_ready = true;

					encoded_something = true;
					cellPngEnc.todo("encoding png done");
				}
			}

			if (!encoded_something)
			{
				thread_ctrl::wait_for(100000);
			}
		}
	}

	static constexpr auto thread_name = "PNG Encoder Thread"sv;

	shared_mutex mutex;
	static u32 ctr;

	struct png_stream
	{
		void abort()
		{
			if (is_encoding)
			{
				state = CELL_PNGENC_ERROR_STREAM_ABORT;
			}
			else if (!is_ready)
			{
				state = CELL_PNGENC_ERROR_STREAM_SKIP;
			}
		}

		bool is_ready = false;
		bool is_encoding = false;
		s32 state = CELL_OK;
		CellPngEncLocation location = CELL_PNGENC_LOCATION_FILE;
		std::string streamFileName;
		vm::bptr<void> streamAddr;
		u32 limitSize = 0;
		u32 streamSize = 0;
		u32 processedLine = 0;
		u64 userData = 0;
		CellPngEncPicture picture{};
		bool enableSpu = false;
		CellPngEncColorSpace encodeColorSpace = CELL_PNGENC_COLOR_SPACE_GRAYSCALE;
		CellPngEncCompressionLevel compressionLevel = CELL_PNGENC_COMPR_LEVEL_0;
		CellPngEncFilterType filterType = CELL_PNGENC_FILTER_TYPE_NONE;
		std::vector<CellPngEncAncillaryChunk> ancillaryChunkList;
	};

	struct encoder_data
	{
		void reset()
		{
			for (png_stream& stream : encode_queue)
			{
				stream.abort();
			}

			is_reset = true;
		}

		CellPngEncConfig config{};
		CellPngEncResource resource{};
		CellPngEncResourceEx resourceEx{};
		bool is_reset = false;
		std::deque<png_stream> encode_queue;
		static constexpr u32 max_queue_size = 100;
	};

	std::map<u32, encoder_data> handles;
};

u32 png_encoder::ctr = 0;

using png_encoder_thread = named_thread<png_encoder>;

bool check_config(vm::cptr<CellPngEncConfig> config)
{
	if (!config ||
		config->maxWidth == 0u || config->maxWidth > 1000000u ||
		config->maxHeight == 0u || config->maxHeight > 1000000u ||
		(config->maxBitDepth != 8u && config->maxBitDepth != 16u) ||
		static_cast<s32>(config->addMemSize) < 0 ||
		config->exParamNum != 0u)
	{
		cellPngEnc.error("maxWidth=*0x%x", config);
		if (!config) return false;
		cellPngEnc.error("maxWidth=%d", config->maxWidth);
		cellPngEnc.error("maxHeight=%d", config->maxHeight);
		cellPngEnc.error("maxBitDepth=%d", config->maxBitDepth);
		cellPngEnc.error("addMemSize=%d", config->addMemSize);
		cellPngEnc.error("exParamNum=%d", config->exParamNum);
		return false;
	}

	return true;
}

u32 get_mem_size(vm::cptr<CellPngEncConfig> config)
{
	return config->addMemSize
		+ (config->enableSpu ? 0x78200 : 0x47a00)
		+ (config->maxBitDepth >> 1) * config->maxWidth * 7;
}


error_code cellPngEncQueryAttr(vm::cptr<CellPngEncConfig> config, vm::ptr<CellPngEncAttr> attr)
{
	cellPngEnc.todo("cellPngEncQueryAttr(config=*0x%x, attr=*0x%x)", config, attr);

	if (!attr || !check_config(config))
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	const u32 memsize = get_mem_size(config);
	attr->memSize = memsize + 0x1780;
	attr->cmdQueueDepth = 4;
	attr->versionLower = 0;
	attr->versionUpper = 0x270000;

	return CELL_OK;
}

error_code cellPngEncOpen(vm::cptr<CellPngEncConfig> config, vm::cptr<CellPngEncResource> resource, vm::ptr<u32> handle)
{
	cellPngEnc.todo("cellPngEncOpen(config=*0x%x, resource=*0x%x, handle=0x%x)", config, resource, handle);

	if (!handle || !check_config(config) ||
		!resource || !resource->memAddr || !resource->memSize ||
		resource->ppuThreadPriority < 0 || resource->ppuThreadPriority > 0xbff ||
		resource->spuThreadPriority < 0 || resource->spuThreadPriority > 0xff)
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	const u32 required_memsize = get_mem_size(config);

	if (resource->memSize < required_memsize + 0x1780U)
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	auto& encoder = g_fxo->get<png_encoder_thread>();
	{
		std::lock_guard lock(encoder.mutex);

		if (png_encoder::ctr == 0)
			png_encoder::ctr = 1;

		*handle = png_encoder::ctr++;

		png_encoder::encoder_data& data = encoder.handles[*handle];
		data.config = *config;
		data.resource = *resource;
	}

	return CELL_OK;
}

error_code cellPngEncOpenEx(vm::cptr<CellPngEncConfig> config, vm::cptr<CellPngEncResourceEx> resource, vm::ptr<u32> handle)
{
	cellPngEnc.todo("cellPngEncOpenEx(config=*0x%x, resourceEx=*0x%x, handle=0x%x)", config, resource, handle);

	if (!handle || !check_config(config) ||
		!resource || !resource->memAddr || !resource->memSize ||
		resource->ppuThreadPriority < 0 || resource->ppuThreadPriority > 0xbff ||
		resource->priority[0] > 15 || resource->priority[1] > 15 ||
		resource->priority[2] > 15 || resource->priority[3] > 15 ||
		resource->priority[4] > 15 || resource->priority[5] > 15 ||
		resource->priority[6] > 15 || resource->priority[7] > 15)
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	const u32 required_memsize = get_mem_size(config);

	if (resource->memSize < required_memsize + 0x1780U)
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	auto& encoder = g_fxo->get<png_encoder_thread>();
	{
		std::lock_guard lock(encoder.mutex);

		if (png_encoder::ctr == 0)
			png_encoder::ctr = 1;

		*handle = png_encoder::ctr++;

		png_encoder::encoder_data& data = encoder.handles[*handle];
		data.config = *config;
		data.resourceEx = *resource;
	}

	return CELL_OK;
}

error_code cellPngEncClose(u32 handle)
{
	cellPngEnc.todo("cellPngEncClose(handle=0x%x)", handle);

	if (!handle)
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	auto& encoder = g_fxo->get<png_encoder_thread>();

	std::lock_guard lock(encoder.mutex);
	{
		if (!encoder.handles.contains(handle))
		{
			return CELL_PNGENC_ERROR_ARG;
		}

		encoder.handles[handle].reset();
		encoder.handles.erase(handle);
	}

	return CELL_OK;
}

error_code cellPngEncWaitForInput(ppu_thread& ppu, u32 handle, b8 block)
{
	cellPngEnc.todo("cellPngEncWaitForInput(handle=0x%x, block=%d)", handle, block);

	if (!handle)
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	auto& encoder = g_fxo->get<png_encoder_thread>();

	std::unique_lock lock(encoder.mutex);

	if (!encoder.handles.contains(handle))
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	if (block)
	{
		lock.unlock();

		while (true)
		{
			lock.lock();

			if (!encoder.handles.contains(handle))
			{
				return CELL_PNGENC_ERROR_FATAL;
			}

			const png_encoder::encoder_data& data = encoder.handles[handle];

			if (data.is_reset)
			{
				return CELL_PNGENC_ERROR_RESET;
			}

			if (data.encode_queue.size() < data.max_queue_size)
			{
				return CELL_OK;
			}

			lock.unlock();

			thread_ctrl::wait_for(100);
		}
	}
	else
	{
		const png_encoder::encoder_data& data = encoder.handles[handle];

		if (data.encode_queue.size() >= data.max_queue_size)
		{
			return CELL_PNGENC_ERROR_BUSY;
		}
	}

	return CELL_OK;
}

error_code cellPngEncEncodePicture(u32 handle, vm::cptr<CellPngEncPicture> picture, vm::cptr<CellPngEncEncodeParam> encodeParam, vm::cptr<CellPngEncOutputParam> outputParam)
{
	cellPngEnc.todo("cellPngEncEncodePicture(handle=0x%x, picture=*0x%x, encodeParam=*0x%x, outputParam=*0x%x)", handle, picture, encodeParam, outputParam);

	if (!handle || !picture || !picture->width || !picture->height ||
		(picture->packedPixel && picture->bitDepth >= 8) ||
		!picture->pictureAddr || picture->colorSpace > CELL_PNGENC_COLOR_SPACE_ARGB)
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	// TODO: check this
	if (encodeParam->enableSpu)
	{
		switch (picture->bitDepth)
		{
		case 1:
		case 2:
		case 4:
		case 8:
			if (picture->width > 2560)
				return CELL_PNGENC_ERROR_ARG;
			break;
		case 16:
			if (picture->width > 1280)
				return CELL_PNGENC_ERROR_ARG;
			break;
		default:
			return CELL_PNGENC_ERROR_ARG;
		}
	}

	// TODO: check this
	switch (picture->colorSpace)
	{
	case CELL_PNGENC_COLOR_SPACE_GRAYSCALE:
		if (picture->bitDepth != 1 && picture->bitDepth != 2 && picture->bitDepth != 4 && picture->bitDepth != 8 && picture->bitDepth != 16)
			return CELL_PNGENC_ERROR_ARG;
		break;
	case CELL_PNGENC_COLOR_SPACE_PALETTE:
		if (picture->bitDepth != 1 && picture->bitDepth != 2 && picture->bitDepth != 4 && picture->bitDepth != 8)
			return CELL_PNGENC_ERROR_ARG;
		break;
	case CELL_PNGENC_COLOR_SPACE_RGB:
	case CELL_PNGENC_COLOR_SPACE_GRAYSCALE_ALPHA:
	case CELL_PNGENC_COLOR_SPACE_RGBA:
	case CELL_PNGENC_COLOR_SPACE_ARGB:
		if (picture->bitDepth != 8 && picture->bitDepth != 16)
			return CELL_PNGENC_ERROR_ARG;
		break;
	default:
		return CELL_PNGENC_ERROR_ARG;
	}

	// TODO: check this
	if (picture->bitDepth >= 8 && picture->packedPixel)
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	auto& encoder = g_fxo->get<png_encoder_thread>();
	{
		std::lock_guard lock(encoder.mutex);

		if (!encoder.handles.contains(handle))
		{
			return CELL_PNGENC_ERROR_ARG;
		}

		png_encoder::encoder_data& data = encoder.handles[handle];

		if (picture->width > data.config.maxWidth ||
			picture->height > data.config.maxHeight ||
			picture->bitDepth > data.config.maxBitDepth)
		{
			return CELL_PNGENC_ERROR_ARG;
		}

		png_encoder::png_stream stream {};
		stream.picture = *picture;
		stream.enableSpu = encodeParam->enableSpu;
		stream.encodeColorSpace = static_cast<CellPngEncColorSpace>(u32(encodeParam->encodeColorSpace));
		stream.compressionLevel = static_cast<CellPngEncCompressionLevel>(u32(encodeParam->compressionLevel));
		stream.filterType = static_cast<CellPngEncFilterType>(u32(encodeParam->filterType));
		stream.ancillaryChunkList.resize(encodeParam->ancillaryChunkNum);
		for (u32 i = 0; i < ::size32(stream.ancillaryChunkList); i++)
		{
			stream.ancillaryChunkList[i] = encodeParam->ancillaryChunkList[i];
		}
		stream.location = static_cast<CellPngEncLocation>(u32(outputParam->location));
		if (stream.location == CellPngEncLocation::CELL_PNGENC_LOCATION_FILE && outputParam->streamFileName)
		{
			stream.streamFileName = outputParam->streamFileName.get_ptr();
		}
		stream.streamAddr = outputParam->streamAddr;
		stream.limitSize = outputParam->limitSize;
		stream.userData = picture->userData;
		stream.is_encoding = true;
		data.encode_queue.push_back(std::move(stream));
	}

	return CELL_OK;
}

error_code cellPngEncWaitForOutput(u32 handle, vm::ptr<u32> streamInfoNum, b8 block)
{
	cellPngEnc.todo("cellPngEncWaitForOutput(handle=0x%x, streamInfoNum=*0x%x, block=%d)", handle, streamInfoNum, block);

	if (!handle || !streamInfoNum)
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	auto& encoder = g_fxo->get<png_encoder_thread>();

	std::unique_lock lock(encoder.mutex);

	if (!encoder.handles.contains(handle))
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	const auto count_ready_streams = [](const png_encoder::encoder_data& data)
	{
		u32 streams_ready = 0;

		for (const png_encoder::png_stream& stream : data.encode_queue)
		{
			if (!stream.is_ready) break;
			streams_ready++;
		}

		return streams_ready;
	};

	if (block)
	{
		lock.unlock();

		while (true)
		{
			lock.lock();

			if (!encoder.handles.contains(handle))
			{
				return CELL_PNGENC_ERROR_FATAL;
			}

			const png_encoder::encoder_data& data = encoder.handles[handle];

			if (data.is_reset)
			{
				return CELL_PNGENC_ERROR_RESET;
			}

			const u32 streams_ready = count_ready_streams(data);

			if (streams_ready)
			{
				*streamInfoNum = streams_ready;
				return CELL_OK;
			}

			lock.unlock();

			thread_ctrl::wait_for(100);
		}
	}
	else
	{
		*streamInfoNum = count_ready_streams(encoder.handles[handle]);
	}

	return CELL_OK;
}

error_code cellPngEncGetStreamInfo(u32 handle, vm::ptr<CellPngEncStreamInfo> streamInfo)
{
	cellPngEnc.todo("cellPngEncGetStreamInfo(handle=0x%x, streamInfo=*0x%x)", handle, streamInfo);

	if (!handle || !streamInfo)
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	auto& encoder = g_fxo->get<png_encoder_thread>();
	{
		std::lock_guard lock(encoder.mutex);

		if (!encoder.handles.contains(handle))
		{
			return CELL_PNGENC_ERROR_ARG;
		}

		png_encoder::encoder_data& data = encoder.handles[handle];

		for (const png_encoder::png_stream& stream : data.encode_queue)
		{
			if (!stream.is_ready) break;

			*streamInfo = {};
			streamInfo->state = stream.state;
			streamInfo->location = stream.location;

			if (stream.location == CELL_PNGENC_LOCATION_FILE)
			{
				streamInfo->streamFileName = vm::make_str(stream.streamFileName);
			}
			streamInfo->streamAddr = stream.streamAddr;
			streamInfo->limitSize = stream.limitSize;
			streamInfo->streamSize = stream.streamSize;
			streamInfo->processedLine = stream.processedLine;
			streamInfo->userData = stream.userData;
			data.encode_queue.pop_front();
			return CELL_OK;
		}
	}

	return CELL_PNGENC_ERROR_EMPTY;
}

error_code cellPngEncReset(u32 handle)
{
	cellPngEnc.todo("cellPngEncReset(handle=0x%x)", handle);

	if (!handle)
	{
		return CELL_PNGENC_ERROR_ARG;
	}

	auto& encoder = g_fxo->get<png_encoder_thread>();
	{
		std::lock_guard lock(encoder.mutex);

		if (!encoder.handles.contains(handle))
		{
			return CELL_PNGENC_ERROR_ARG;
		}

		encoder.handles[handle].reset();
	}

	return CELL_OK;
}

DECLARE(ppu_module_manager::cellPngEnc)("cellPngEnc", []()
{
	REG_FUNC(cellPngEnc, cellPngEncQueryAttr);
	REG_FUNC(cellPngEnc, cellPngEncOpen);
	REG_FUNC(cellPngEnc, cellPngEncOpenEx);
	REG_FUNC(cellPngEnc, cellPngEncClose);
	REG_FUNC(cellPngEnc, cellPngEncWaitForInput);
	REG_FUNC(cellPngEnc, cellPngEncEncodePicture);
	REG_FUNC(cellPngEnc, cellPngEncWaitForOutput);
	REG_FUNC(cellPngEnc, cellPngEncGetStreamInfo);
	REG_FUNC(cellPngEnc, cellPngEncReset);
});
