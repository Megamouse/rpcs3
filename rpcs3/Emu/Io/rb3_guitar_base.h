#pragma once

#include "Emu/Io/usb_device.h"
#include <array>

class rb3_guitar_base : public usb_device_emulated
{
public:
	rb3_guitar_base(const std::array<u8, 7>& location, bool twentytwo_fret);
	virtual ~rb3_guitar_base();

	void control_transfer(u8 bmRequestType, u8 bRequest, u16 wValue, u16 wIndex, u16 wLength, u32 buf_size, u8* buf, UsbTransfer* transfer) override;
	void interrupt_transfer(u32 buf_size, u8* buf, u32 endpoint, UsbTransfer* transfer) override;

private:
	void write_state(u8* buf, u32 buf_size);

protected:
	virtual void get_input() = 0;

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

	bool m_buttons_enabled = false;
	usz m_response_pos = 0;

	// button states
	struct
	{
		u8 count = 0;

		bool cross = false;
		bool circle = false;
		bool square = false;
		bool triangle = false;

		bool start = false;
		bool select = false;
		bool tilt_sensor = false;
		bool sustain_pedal = false; // used for overdrive

		u8 dpad = 8;

		std::array<u8, 6> frets{};
		std::array<u8, 6> string_velocities{};
	} m_button_state{};
};
