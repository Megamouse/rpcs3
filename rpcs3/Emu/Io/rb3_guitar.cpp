// RB3 guitar controller emulator

#include "stdafx.h"
#include "rb3_guitar.h"
#include "Emu/Cell/lv2/sys_usbd.h"
#include "Emu/Io/rb3_guitar_config.h"
#include "Input/pad_thread.h"

LOG_CHANNEL(rb3_guitar_log, "RB3Guitar");

template <>
void fmt_class_string<rb3_guitar_btn>::format(std::string& out, u64 arg)
{
	format_enum(out, arg, [](rb3_guitar_btn value)
	{
		switch (value)
		{
		case rb3_guitar_btn::dpad_up: return "D-Pad Up";
		case rb3_guitar_btn::dpad_down: return "D-Pad Down";
		case rb3_guitar_btn::dpad_left: return "D-Pad Left";
		case rb3_guitar_btn::dpad_right: return "D-Pad Right";
		case rb3_guitar_btn::cross: return "Cross";
		case rb3_guitar_btn::circle: return "Circle";
		case rb3_guitar_btn::square: return "Square";
		case rb3_guitar_btn::triangle: return "Triangle";
		case rb3_guitar_btn::start: return "Start";
		case rb3_guitar_btn::select: return "Select";
		case rb3_guitar_btn::tilt: return "Tilt";
		case rb3_guitar_btn::sustain_pedal: return "Sustain Pedal";
		case rb3_guitar_btn::fret_00: return "Fret 0";
		case rb3_guitar_btn::fret_01: return "Fret 1";
		case rb3_guitar_btn::fret_02: return "Fret 2";
		case rb3_guitar_btn::fret_03: return "Fret 3";
		case rb3_guitar_btn::fret_04: return "Fret 4";
		case rb3_guitar_btn::fret_05: return "Fret 5";
		case rb3_guitar_btn::fret_06: return "Fret 6";
		case rb3_guitar_btn::fret_07: return "Fret 7";
		case rb3_guitar_btn::fret_08: return "Fret 8";
		case rb3_guitar_btn::fret_09: return "Fret 9";
		case rb3_guitar_btn::fret_10: return "Fret 10";
		case rb3_guitar_btn::fret_11: return "Fret 11";
		case rb3_guitar_btn::fret_12: return "Fret 12";
		case rb3_guitar_btn::fret_13: return "Fret 13";
		case rb3_guitar_btn::fret_14: return "Fret 14";
		case rb3_guitar_btn::fret_15: return "Fret 15";
		case rb3_guitar_btn::fret_16: return "Fret 16";
		case rb3_guitar_btn::fret_17: return "Fret 17";
		case rb3_guitar_btn::fret_18: return "Fret 18";
		case rb3_guitar_btn::fret_19: return "Fret 19";
		case rb3_guitar_btn::fret_20: return "Fret 20";
		case rb3_guitar_btn::fret_21: return "Fret 21";
		case rb3_guitar_btn::count: return "Count";
		}

		return unknown;
	});
}

static constexpr std::array<u8, 40> disabled_response = {
	0xe9, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0f, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x82,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x21, 0x26, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00};

static constexpr std::array<u8, 40> enabled_response = {
	0xe9, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x8a,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x21, 0x26, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00};

usb_device_rb3_guitar::usb_device_rb3_guitar(u32 controller_index, const std::array<u8, 7>& location, bool twentytwo_fret)
	: rb3_guitar_base(location, twentytwo_fret)
	, m_controller_index(controller_index)
{
}

usb_device_rb3_guitar::~usb_device_rb3_guitar()
{
}

void usb_device_rb3_guitar::get_input()
{
	std::lock_guard lock(pad::g_pad_mutex);
	const auto handler = pad::get_current_handler();
	const auto& pad    = ::at32(handler->GetPads(), m_controller_index);

	if (!(pad->m_port_status & CELL_PAD_STATUS_CONNECTED))
	{
		return;
	}

	const auto& cfg = ::at32(g_cfg_rb3_guitar.players, m_controller_index);
	cfg->handle_input(pad, true, [this](rb3_guitar_btn btn, u16 value, bool pressed)
	{
		if (!pressed)
			return;

		switch (btn)
		{
		case rb3_guitar_btn::cross:
			m_button_state.cross = true;
			break;
		case rb3_guitar_btn::circle:
			m_button_state.circle = true;
			break;
		case rb3_guitar_btn::square:
			m_button_state.square = true;
			break;
		case rb3_guitar_btn::triangle:
			m_button_state.triangle = true;
			break;
		case rb3_guitar_btn::start:
			m_button_state.start = true;
			break;
		case rb3_guitar_btn::select:
			m_button_state.select = true;
			break;
		case rb3_guitar_btn::tilt:
			m_button_state.tilt_sensor = true;
			break;
		case rb3_guitar_btn::sustain_pedal:
			m_button_state.sustain_pedal = true;
			break;
		case rb3_guitar_btn::dpad_up:
		case rb3_guitar_btn::dpad_down:
		case rb3_guitar_btn::dpad_left:
		case rb3_guitar_btn::dpad_right:
			//m_button_state.dpad |= TODO;
			break;
		case rb3_guitar_btn::fret_00:
		case rb3_guitar_btn::fret_01:
		case rb3_guitar_btn::fret_02:
		case rb3_guitar_btn::fret_03:
		case rb3_guitar_btn::fret_04:
		case rb3_guitar_btn::fret_05:
		case rb3_guitar_btn::fret_06:
		case rb3_guitar_btn::fret_07:
		case rb3_guitar_btn::fret_08:
		case rb3_guitar_btn::fret_09:
		case rb3_guitar_btn::fret_10:
		case rb3_guitar_btn::fret_11:
		case rb3_guitar_btn::fret_12:
		case rb3_guitar_btn::fret_13:
		case rb3_guitar_btn::fret_14:
		case rb3_guitar_btn::fret_15:
		case rb3_guitar_btn::fret_16:
		case rb3_guitar_btn::fret_17:
		case rb3_guitar_btn::fret_18:
		case rb3_guitar_btn::fret_19:
		case rb3_guitar_btn::fret_20:
		case rb3_guitar_btn::fret_21:
			//m_button_state.frets |= TODO;
			break;
		case rb3_guitar_btn::count:
			break;
		}
	});
}
