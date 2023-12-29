#pragma once

#include "rb3_guitar_base.h"

class usb_device_rb3_guitar : public rb3_guitar_base
{
public:
	usb_device_rb3_guitar(u32 controller_index, const std::array<u8, 7>& location, bool twentytwo_fret);
	virtual ~usb_device_rb3_guitar();

protected:
	void get_input() override;

private:
	u32 m_controller_index = 0;
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
