// Rock Band 3 MIDI Pro Adapter Emulator (Guitar Mode)

#include "stdafx.h"
#include "RB3MidiGuitar.h"

LOG_CHANNEL(rb3_midi_guitar_log);

usb_device_rb3_midi_guitar::usb_device_rb3_midi_guitar(const std::array<u8, 7>& location, const std::string& device_name, bool twentytwo_fret)
	: rb3_guitar_base(location, twentytwo_fret)
{
	// connect to midi device
	midi_in = rtmidi_in_create_default();
	ensure(midi_in);

	if (!midi_in->ok)
	{
		rb3_midi_guitar_log.error("Could not get MIDI in ptr: %s", midi_in->msg);
		return;
	}

	const RtMidiApi api = rtmidi_in_get_current_api(midi_in);

	if (!midi_in->ok)
	{
		rb3_midi_guitar_log.error("Could not get MIDI api: %s", midi_in->msg);
		return;
	}

	if (const char* api_name = rtmidi_api_name(api))
	{
		rb3_midi_guitar_log.notice("Using %s api", api_name);
	}
	else
	{
		rb3_midi_guitar_log.warning("Could not get MIDI api name");
	}

	rtmidi_in_ignore_types(midi_in, false, true, true);

	const u32 port_count = rtmidi_get_port_count(midi_in);

	if (!midi_in->ok || port_count == umax)
	{
		rb3_midi_guitar_log.error("Could not get MIDI port count: %s", midi_in->msg);
		return;
	}

	for (u32 port_number = 0; port_number < port_count; port_number++)
	{
		char buf[128]{};
		s32 size = sizeof(buf);
		if (rtmidi_get_port_name(midi_in, port_number, buf, &size) == -1 || !midi_in->ok)
		{
			rb3_midi_guitar_log.error("Error getting port name for port %d: %s", port_number, midi_in->msg);
			return;
		}

		rb3_midi_guitar_log.notice("Found device with name: %s", buf);

		if (device_name == buf)
		{
			rtmidi_open_port(midi_in, port_number, "RPCS3 MIDI Guitar Input");

			if (!midi_in->ok)
			{
				rb3_midi_guitar_log.error("Could not open port %d for device '%s': %s", port_number, device_name, midi_in->msg);
				return;
			}

			rb3_midi_guitar_log.success("Connected to device: %s", device_name);
			return;
		}
	}

	rb3_midi_guitar_log.error("Could not find device with name: %s", device_name);
}

usb_device_rb3_midi_guitar::~usb_device_rb3_midi_guitar()
{
	rtmidi_in_free(midi_in);
}

void usb_device_rb3_midi_guitar::get_input()
{
	while (true)
	{
		u8 midi_msg[32];
		usz size = sizeof(midi_msg);

		// this returns a double as some sort of delta time, with -1.0
		// being used to signal an error
		if (rtmidi_in_get_message(midi_in, midi_msg, &size) == -1.0)
		{
			rb3_midi_guitar_log.error("Error getting MIDI message: %s", midi_in->msg);
			return;
		}

		if (size == 0)
		{
			break;
		}

		parse_midi_message(midi_msg, size);
	}
}

void usb_device_rb3_midi_guitar::parse_midi_message(u8* msg, usz size)
{
	// this is not emulated correctly but the game doesn't seem to care
	m_button_state.count++;

	// read frets
	if (size == 8 && msg[0] == 0xF0 && msg[4] == 0x01)
	{
		switch (msg[5])
		{
		case 1:
			m_button_state.frets[0] = msg[6] - 0x40;
			break;
		case 2:
			m_button_state.frets[1] = msg[6] - 0x3B;
			break;
		case 3:
			m_button_state.frets[2] = msg[6] - 0x37;
			break;
		case 4:
			m_button_state.frets[3] = msg[6] - 0x32;
			break;
		case 5:
			m_button_state.frets[4] = msg[6] - 0x2D;
			break;
		case 6:
			m_button_state.frets[5] = msg[6] - 0x28;
			break;
		default:
			rb3_midi_guitar_log.warning("Invalid string for fret event: %d", msg[5]);
			break;
		}
	}

	// read strings
	if (size == 8 && msg[0] == 0xF0 && msg[4] == 0x05)
	{
		m_button_state.string_velocities[msg[5] - 1] = msg[6];
	}

	// read buttons
	if (size == 10 && msg[0] == 0xF0 && msg[4] == 0x08)
	{
		m_button_state.dpad = msg[7] & 0x0f;

		m_button_state.square = (msg[5] & 0b0000'0001) == 0b0000'0001;
		m_button_state.cross = (msg[5] & 0b0000'0010) == 0b0000'0010;
		m_button_state.circle = (msg[5] & 0b0000'0100) == 0b0000'0100;
		m_button_state.triangle = (msg[5] & 0b0000'1000) == 0b0000'1000;

		m_button_state.select = (msg[6] & 0b0000'0001) == 0b0000'0001;
		m_button_state.start = (msg[6] & 0b0000'0010) == 0b0000'0010;
		m_button_state.tilt_sensor = (msg[7] & 0b0100'0000) == 0b0100'0000;
	}

	// sustain pedal
	if (size == 3 && msg[0] == 0xB0 && msg[1] == 0x40)
	{
		m_button_state.sustain_pedal = msg[2] >= 40;
	}
}
