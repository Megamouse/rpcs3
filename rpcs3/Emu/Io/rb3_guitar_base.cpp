#include "stdafx.h"
#include "rb3_guitar_base.h"
#include "Emu/Cell/lv2/sys_usbd.h"

LOG_CHANNEL(rb3_guitar_log, "RB3Guitar");

extern bool is_input_allowed();

rb3_guitar_base::rb3_guitar_base(const std::array<u8, 7>& location, bool twentytwo_fret)
	: usb_device_emulated(location)
{
	// For the 22-fret guitar (Fender Squier), the only thing that's different
	// is the device ID reported by the MIDI Pro Adapter.
	// Everything else is *exactly* the same as the 17-fret guitar (Fender Mustang).
	if (twentytwo_fret)
	{
		device = UsbDescriptorNode(USB_DESCRIPTOR_DEVICE, UsbDeviceDescriptor{0x0200, 0x00, 0x00, 0x00, 64, 0x12ba, 0x2538, 0x01, 0x01, 0x02, 0x00, 0x01});
	}
	else
	{
		device = UsbDescriptorNode(USB_DESCRIPTOR_DEVICE, UsbDeviceDescriptor{0x0200, 0x00, 0x00, 0x00, 64, 0x12ba, 0x2438, 0x01, 0x01, 0x02, 0x00, 0x01});
	}

	auto& config0 = device.add_node(UsbDescriptorNode(USB_DESCRIPTOR_CONFIG, UsbDeviceConfiguration{41, 1, 1, 0, 0x80, 32}));
	config0.add_node(UsbDescriptorNode(USB_DESCRIPTOR_INTERFACE, UsbDeviceInterface{0, 0, 2, 3, 0, 0, 0}));
	config0.add_node(UsbDescriptorNode(USB_DESCRIPTOR_HID, UsbDeviceHID{0x0111, 0x00, 0x01, 0x22, 137}));
	config0.add_node(UsbDescriptorNode(USB_DESCRIPTOR_ENDPOINT, UsbDeviceEndpoint{0x81, 0x03, 0x0040, 10}));
	config0.add_node(UsbDescriptorNode(USB_DESCRIPTOR_ENDPOINT, UsbDeviceEndpoint{0x02, 0x03, 0x0040, 10}));

	usb_device_emulated::add_string("Licensed by Sony Computer Entertainment America");
	usb_device_emulated::add_string("Harmonix RB3 MIDI Guitar Interface for PlayStation®3");
}

rb3_guitar_base::~rb3_guitar_base()
{
}

void rb3_guitar_base::control_transfer(u8 bmRequestType, u8 bRequest, u16 wValue, u16 wIndex, u16 wLength, u32 buf_size, u8* buf, UsbTransfer* transfer)
{
	transfer->fake = true;

	// configuration packets sent by rock band 3
	// we only really need to check 1 byte here to figure out if the game
	// wants to enable midi data or disable it
	if (bmRequestType == 0x21 && bRequest == 0x9 && wLength == 40)
	{
		if (buf_size < 3)
		{
			rb3_guitar_log.warning("buffer size < 3, bailing out early (buf_size=0x%x)", buf_size);
			return;
		}

		switch (buf[2])
		{
		case 0x89:
			rb3_guitar_log.notice("Button data enabled.");
			m_buttons_enabled = true;
			m_response_pos = 0;
			break;
		case 0x81:
			rb3_guitar_log.notice("Button data disabled.");
			m_buttons_enabled = false;
			m_response_pos = 0;
			break;
		default:
			rb3_guitar_log.warning("Unhandled SET_REPORT request: 0x%02X");
			break;
		}
	}
	// the game expects some sort of response to the configuration packet
	else if (bmRequestType == 0xa1 && bRequest == 0x1)
	{
		transfer->expected_count = buf_size;
		if (m_buttons_enabled)
		{
			const usz remaining_bytes = enabled_response.size() - m_response_pos;
			const usz copied_bytes = std::min<usz>(remaining_bytes, buf_size);
			std::memcpy(buf, &enabled_response[m_response_pos], copied_bytes);
			m_response_pos += copied_bytes;
		}
		else
		{
			const usz remaining_bytes = disabled_response.size() - m_response_pos;
			const usz copied_bytes = std::min<usz>(remaining_bytes, buf_size);
			std::memcpy(buf, &disabled_response[m_response_pos], copied_bytes);
			m_response_pos += copied_bytes;
		}
	}
	else if (bmRequestType == 0x21 && bRequest == 0x9 && wLength == 8)
	{
		// the game uses this request to do things like set the LEDs
		// we don't have any LEDs, so do nothing
	}
	else
	{
		usb_device_emulated::control_transfer(bmRequestType, bRequest, wValue, wIndex, wLength, buf_size, buf, transfer);
	}
}

void rb3_guitar_base::interrupt_transfer(u32 buf_size, u8* buf, u32 /*endpoint*/, UsbTransfer* transfer)
{
	transfer->fake = true;
	transfer->expected_count = buf_size;
	transfer->expected_result = HC_CC_NOERR;
	// the real device takes 8ms to send a response, but there is
	// no reason we can't make it faster
	transfer->expected_time = get_timestamp() + 1'000;

	// default input state
	static constexpr std::array<u8, 27> default_bytes = {
		0x00, 0x00, 0x08, 0x80, 0x80, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
		0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00};

	if (buf_size < default_bytes.size())
	{
		rb3_guitar_log.warning("buffer size < %x, bailing out early (buf_size=0x%x)", default_bytes.size(), buf_size);
		return;
	}

	std::memcpy(buf, default_bytes.data(), default_bytes.size());

	if (!is_input_allowed())
	{
		return;
	}

	get_input();
	write_state(buf, buf_size);
}

void rb3_guitar_base::write_state(u8* buf, u32 buf_size)
{
	constexpr u32 max_bytes_to_write = 18;

	if (!buf || buf_size < max_bytes_to_write)
	{
		rb3_guitar_log.warning("buffer size < %x, bailing out early (buf_size=0x%x)", max_bytes_to_write, buf_size);
		return;
	}

	// encode frets
	buf[8] |= (m_button_state.frets[0] & 0b11111) << 2;
	buf[8] |= (m_button_state.frets[1] & 0b11000) >> 3;
	buf[7] |= (m_button_state.frets[1] & 0b00111) << 5;
	buf[7] |= (m_button_state.frets[2] & 0b11111) >> 0;
	buf[6] |= (m_button_state.frets[3] & 0b11111) << 2;
	buf[6] |= (m_button_state.frets[4] & 0b11000) >> 3;
	buf[5] |= (m_button_state.frets[4] & 0b00111) << 5;
	buf[5] |= (m_button_state.frets[5] & 0b11111) >> 0;

	// encode strings
	buf[14] = m_button_state.string_velocities[0];
	buf[13] = m_button_state.string_velocities[1];
	buf[12] = m_button_state.string_velocities[2];
	buf[11] = m_button_state.string_velocities[3];
	buf[10] = m_button_state.string_velocities[4];
	buf[9] = m_button_state.string_velocities[5];

	// encode frets for playing 5 fret on the pro guitar
	// this actually isn't done by the real MPA, but Rock Band 3 allows this
	// so there's no harm in supporting it.
	for (u8 i : m_button_state.frets)
	{
		switch (i)
		{
			case 1:
			case 6:
			case 13:
				buf[9] |= 0b1000'0000;
				break;
			case 2:
			case 7:
			case 14:
				buf[10] |= 0b1000'0000;
				break;
			case 3:
			case 8:
			case 15:
				buf[11] |= 0b1000'0000;
				break;
			case 4:
			case 9:
			case 16:
				buf[12] |= 0b1000'0000;
				break;
			case 5:
			case 10:
			case 17:
				buf[13] |= 0b1000'0000;
				break;
			default:
				break;
		}

		// enable the solo bit for frets >= 13
		if (i >= 13)
		{
			buf[8] |= 0b1000'0000;
		}
	}

	// encode tilt sensor/sustain_pedal
	if (m_button_state.tilt_sensor || m_button_state.sustain_pedal)
	{
		buf[15] = 0x7f;
		buf[16] = 0x7f;
		buf[17] = 0x7f;
	}

	buf[1] |= 0b0000'0001 * m_button_state.select;
	buf[1] |= 0b0000'0010 * m_button_state.start;

	buf[0] |= 0b0000'0010 * m_button_state.cross;
	buf[0] |= 0b0000'0100 * m_button_state.circle;
	buf[0] |= 0b0000'1000 * m_button_state.triangle;
	buf[0] |= 0b0000'0001 * m_button_state.square;

	buf[2] = m_button_state.dpad;
}
