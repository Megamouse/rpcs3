#pragma once

#include "rb3_guitar_base.h"

#include <rtmidi_c.h>

class usb_device_rb3_midi_guitar : public rb3_guitar_base
{
public:
	usb_device_rb3_midi_guitar(const std::array<u8, 7>& location, const std::string& device_name, bool twentytwo_fret);
	virtual ~usb_device_rb3_midi_guitar();

protected:
	void get_input() override;

private:
	RtMidiInPtr midi_in{};

	void parse_midi_message(u8* msg, usz size);
};
