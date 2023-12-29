#pragma once

#include "emulated_pad_config.h"

#include <array>

enum class rb3_guitar_btn
{
	dpad_up,
	dpad_down,
	dpad_left,
	dpad_right,

	cross,
	circle,
	square,
	triangle,
	start,
	select,

	tilt,
	sustain_pedal,

	fret_00,
	fret_01,
	fret_02,
	fret_03,
	fret_04,
	fret_05,
	fret_06,
	fret_07,
	fret_08,
	fret_09,
	fret_10,
	fret_11,
	fret_12,
	fret_13,
	fret_14,
	fret_15,
	fret_16,
	fret_17,
	fret_18,
	fret_19,
	fret_20,
	fret_21,

	count
};

struct cfg_rb3_guitar final : public emulated_pad_config<rb3_guitar_btn>
{
	cfg_rb3_guitar(node* owner, const std::string& name) : emulated_pad_config(owner, name) {}

	cfg::_bool twenty_two_fret{ this, "22-Fret", false };

	cfg_pad_btn<rb3_guitar_btn> dpad_up{ this, "D-Pad Up", rb3_guitar_btn::dpad_up, pad_button::dpad_up };
	cfg_pad_btn<rb3_guitar_btn> dpad_down{ this, "D-Pad Down", rb3_guitar_btn::dpad_down, pad_button::dpad_down };
	cfg_pad_btn<rb3_guitar_btn> dpad_left{ this, "D-Pad Left", rb3_guitar_btn::dpad_left, pad_button::dpad_left };
	cfg_pad_btn<rb3_guitar_btn> dpad_right{ this, "D-Pad Right", rb3_guitar_btn::dpad_right, pad_button::dpad_right };
	cfg_pad_btn<rb3_guitar_btn> cross{ this, "Cross", rb3_guitar_btn::cross, pad_button::cross };
	cfg_pad_btn<rb3_guitar_btn> circle{ this, "Circle", rb3_guitar_btn::circle, pad_button::circle };
	cfg_pad_btn<rb3_guitar_btn> square{ this, "Square", rb3_guitar_btn::square, pad_button::square };
	cfg_pad_btn<rb3_guitar_btn> triangle{ this, "Triangle", rb3_guitar_btn::triangle, pad_button::triangle };
	cfg_pad_btn<rb3_guitar_btn> start{ this, "Start", rb3_guitar_btn::start, pad_button::start };
	cfg_pad_btn<rb3_guitar_btn> select{ this, "Select", rb3_guitar_btn::select, pad_button::select };
	cfg_pad_btn<rb3_guitar_btn> tilt{ this, "Tilt", rb3_guitar_btn::tilt, pad_button::R2 };
	cfg_pad_btn<rb3_guitar_btn> sustain_pedal{ this, "Sustain Pedal", rb3_guitar_btn::sustain_pedal, pad_button::L2 };

	cfg_pad_btn<rb3_guitar_btn> fret_00{ this, "Fret 0", rb3_guitar_btn::fret_00, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_01{ this, "Fret 1", rb3_guitar_btn::fret_01, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_02{ this, "Fret 2", rb3_guitar_btn::fret_02, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_03{ this, "Fret 3", rb3_guitar_btn::fret_03, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_04{ this, "Fret 4", rb3_guitar_btn::fret_04, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_05{ this, "Fret 5", rb3_guitar_btn::fret_05, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_06{ this, "Fret 6", rb3_guitar_btn::fret_06, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_07{ this, "Fret 7", rb3_guitar_btn::fret_07, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_08{ this, "Fret 8", rb3_guitar_btn::fret_08, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_09{ this, "Fret 9", rb3_guitar_btn::fret_09, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_10{ this, "Fret 10", rb3_guitar_btn::fret_10, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_11{ this, "Fret 11", rb3_guitar_btn::fret_11, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_12{ this, "Fret 12", rb3_guitar_btn::fret_12, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_13{ this, "Fret 13", rb3_guitar_btn::fret_13, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_14{ this, "Fret 14", rb3_guitar_btn::fret_14, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_15{ this, "Fret 15", rb3_guitar_btn::fret_15, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_16{ this, "Fret 16", rb3_guitar_btn::fret_16, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_17{ this, "Fret 17", rb3_guitar_btn::fret_17, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_18{ this, "Fret 18", rb3_guitar_btn::fret_18, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_19{ this, "Fret 19", rb3_guitar_btn::fret_19, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_20{ this, "Fret 20", rb3_guitar_btn::fret_20, pad_button::pad_button_max_enum };
	cfg_pad_btn<rb3_guitar_btn> fret_21{ this, "Fret 21", rb3_guitar_btn::fret_21, pad_button::pad_button_max_enum };
};

struct cfg_rb3_guitars final : public emulated_pads_config<cfg_rb3_guitar, 3>
{
	cfg_rb3_guitars() : emulated_pads_config<cfg_rb3_guitar, 3>("rb3_guitar") {};
};

extern cfg_rb3_guitars g_cfg_rb3_guitar;
