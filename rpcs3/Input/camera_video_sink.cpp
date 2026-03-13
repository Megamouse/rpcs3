#include "stdafx.h"
#include "camera_video_sink.h"

#include "Emu/Cell/Modules/cellCamera.h"
#include "Emu/system_config.h"

LOG_CHANNEL(camera_log, "Camera");

static constexpr s32 yuv_scale = 256;
static constexpr s32 factor_yr = 0.257f * yuv_scale;
static constexpr s32 factor_yg = 0.504f * yuv_scale;
static constexpr s32 factor_yb = 0.098f * yuv_scale;
static constexpr s32 factor_ur = 0.148f * yuv_scale;
static constexpr s32 factor_ug = 0.291f * yuv_scale;
static constexpr s32 factor_ub = 0.439f * yuv_scale;
static constexpr s32 factor_vr = 0.439f * yuv_scale;
static constexpr s32 factor_vg = 0.368f * yuv_scale;
static constexpr s32 factor_vb = 0.071f * yuv_scale;
static constexpr s32 factor_yf =     16 * yuv_scale;
static constexpr s32 factor_uvf =   128 * yuv_scale;

camera_video_sink::camera_video_sink(bool front_facing)
	: m_front_facing(front_facing)
{
}

camera_video_sink::~camera_video_sink()
{
}

bool camera_video_sink::present(u32 src_width, u32 src_height, u32 src_pitch, u32 src_bytes_per_pixel, std::function<const u8*(u32)> src_line_ptr)
{
	ensure(!!src_line_ptr);

	const u64 new_size = m_bytesize;
	image_buffer& image_buffer = m_image_buffer[m_write_index];

	// Reset buffer if necessary
	if (image_buffer.data.size() != new_size)
	{
		image_buffer.data.clear();
	}

	// Create buffer if necessary
	if (image_buffer.data.empty() && new_size > 0)
	{
		image_buffer.data.resize(new_size);
		image_buffer.width = m_width;
		image_buffer.height = m_height;
	}

	if (!image_buffer.data.empty() && src_width && src_height)
	{
		// Convert image to proper layout
		// TODO: check if pixel format and bytes per pixel match and convert if necessary
		// TODO: implement or improve more conversions

		const u32 width = std::min<u32>(image_buffer.width, src_width);
		const u32 height = std::min<u32>(image_buffer.height, src_height);

		switch (m_format)
		{
		case CELL_CAMERA_RAW8: // The game seems to expect BGGR
		{
			// Let's use a very simple algorithm to convert the image to raw BGGR
			u8* dst = image_buffer.data.data();

			for (u32 y = 0; y < height; y++)
			{
				const u8* src = src_line_ptr(y);
				const u8* srcu = src_line_ptr(std::max<s32>(0, y - 1));
				const u8* srcd = src_line_ptr(std::min(height - 1, y + 1));
				const bool is_top_pixel = (y % 2) == 0;

				// We apply gaussian blur to get better demosaicing results later when debayering again
				const auto blurred = [&](s32 x, s32 c)
				{
					const s32 i = x * 4 + c;
					const s32 il = std::max(0, x - 1) * 4 + c;
					const s32 ir = std::min<s32>(width - 1, x + 1) * 4 + c;
					const s32 sum =
										srcu[i] +
						src[il] + 4 * src[i]  + src[ir] +
										srcd[i];
					return static_cast<u8>(std::clamp((sum + 4) / 8, 0, 255));
				};

				// Split loops (roughly twice the performance by removing one condition)
				if (is_top_pixel)
				{
					for (u32 x = 0; x < width; x++, dst++)
					{
						const bool is_left_pixel = (x % 2) == 0;

						if (is_left_pixel)
						{
							*dst = blurred(x, 2); // Blue
						}
						else
						{
							*dst = blurred(x, 1); // Green
						}
					}
				}
				else
				{
					for (u32 x = 0; x < width; x++, dst++)
					{
						const bool is_left_pixel = (x % 2) == 0;

						if (is_left_pixel)
						{
							*dst = blurred(x, 1); // Green
						}
						else
						{
							*dst = blurred(x, 0); // Red
						}
					}
				}
			}
			break;
		}
		//case CELL_CAMERA_YUV422:
		case CELL_CAMERA_Y0_U_Y1_V:
		case CELL_CAMERA_V_Y1_U_Y0:
		{
			// Simple RGB to Y0_U_Y1_V conversion from stackoverflow.
			constexpr s32 yuv_bytes_per_pixel = 2;
			const s32 yuv_pitch = image_buffer.width * yuv_bytes_per_pixel;

			const s32 y0_offset = (m_format == CELL_CAMERA_Y0_U_Y1_V) ? 0 : 3;
			const s32 u_offset  = (m_format == CELL_CAMERA_Y0_U_Y1_V) ? 1 : 2;
			const s32 y1_offset = (m_format == CELL_CAMERA_Y0_U_Y1_V) ? 2 : 1;
			const s32 v_offset  = (m_format == CELL_CAMERA_Y0_U_Y1_V) ? 3 : 0;

			for (u32 y = 0; y < height; y++)
			{
				const u8* src = src_line_ptr(y);
				u8* dst = &image_buffer.data[y * yuv_pitch];

				for (u32 x = 0; x < width - 1; x += 2, src += 8, dst += 2 * yuv_bytes_per_pixel)
				{
					const u8 r1 = src[0];
					const u8 g1 = src[1];
					const u8 b1 = src[2];
					const u8 r2 = src[4];
					const u8 g2 = src[5];
					const u8 b2 = src[6];

					const u8 y0 = static_cast<u8>(std::clamp(( (factor_yr * r1) + (factor_yg * g1) + (factor_yb * b1) +  factor_yf) >> 8, 0, 255));
					const u8 u  = static_cast<u8>(std::clamp((-(factor_ur * r1) - (factor_ug * g1) + (factor_ub * b1) + factor_uvf) >> 8, 0, 255));
					const u8 v  = static_cast<u8>(std::clamp(( (factor_vr * r1) - (factor_vg * g1) - (factor_vb * b1) + factor_uvf) >> 8, 0, 255));
					const u8 y1 = static_cast<u8>(std::clamp(( (factor_yr * r2) + (factor_yg * g2) + (factor_yb * b2) +  factor_yf) >> 8, 0, 255));

					dst[y0_offset] = y0;
					dst[u_offset]  = u;
					dst[y1_offset] = v;
					dst[v_offset]  = y1;
				}
			}
			break;
		}
		case CELL_CAMERA_JPG:
		case CELL_CAMERA_RGBA:
		case CELL_CAMERA_RAW10:
		case CELL_CAMERA_YUV420:
		case CELL_CAMERA_FORMAT_UNKNOWN:
		default:
			const u32 bytes_per_line = src_bytes_per_pixel * src_width;
			if (src_pitch == bytes_per_line)
			{
				std::memcpy(image_buffer.data.data(), src_line_ptr(0), std::min<usz>(image_buffer.data.size(), src_height * bytes_per_line));
			}
			else
			{
				for (u32 y = 0, pos = 0; y < src_height && pos < image_buffer.data.size(); y++, pos += bytes_per_line)
				{
					std::memcpy(&image_buffer.data[pos], src_line_ptr(y), std::min<usz>(image_buffer.data.size() - pos, bytes_per_line));
				}
			}
			break;
		}
	}

	camera_log.trace("Wrote image to video surface. index=%d, m_frame_number=%d, width=%d, height=%d, bytesize=%d",
		m_write_index, m_frame_number.load(), m_width, m_height, m_bytesize);

	// Toggle write/read index
	std::lock_guard lock(m_mutex);
	image_buffer.frame_number = m_frame_number++;
	m_write_index = read_index();

	return true;
}

void camera_video_sink::set_format(s32 format, u32 bytesize)
{
	camera_log.notice("Setting format: format=%d, bytesize=%d", format, bytesize);

	m_format = format;
	m_bytesize = bytesize;
}

void camera_video_sink::set_resolution(u32 width, u32 height)
{
	camera_log.notice("Setting resolution: width=%d, height=%d", width, height);

	m_width = width;
	m_height = height;
}

void camera_video_sink::set_mirrored(bool mirrored)
{
	camera_log.notice("Setting mirrored: mirrored=%d", mirrored);

	m_mirrored = mirrored;
}

u64 camera_video_sink::frame_number() const
{
	return m_frame_number.load();
}

void camera_video_sink::get_image(u8* buf, u64 size, u32& width, u32& height, u64& frame_number, u64& bytes_read)
{
	// Lock read buffer
	std::lock_guard lock(m_mutex);
	const image_buffer& image_buffer = m_image_buffer[read_index()];

	width = image_buffer.width;
	height = image_buffer.height;
	frame_number = image_buffer.frame_number;

	// Copy to out buffer
	if (buf && !image_buffer.data.empty())
	{
		bytes_read = std::min<u64>(image_buffer.data.size(), size);
		std::memcpy(buf, image_buffer.data.data(), bytes_read);

		if (image_buffer.data.size() != size)
		{
			camera_log.error("Buffer size mismatch: in=%d, out=%d. Cropping to incoming size. Please contact a developer.", size, image_buffer.data.size());
		}
	}
	else
	{
		bytes_read = 0;
	}
}

u32 camera_video_sink::read_index() const
{
	// The read buffer index cannot be the same as the write index
	return (m_write_index + 1u) % ::narrow<u32>(m_image_buffer.size());
}

