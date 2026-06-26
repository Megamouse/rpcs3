#include "stdafx.h"
#include "openxr_handler.h"
#include "openxr_instance.h"

LOG_CHANNEL(openxr_log, "openxr");

openxr_instance g_openxr_instance;

openxr_handler::openxr_handler()
	: PadHandlerBase()
{
	// Unique names for the config files and our pad settings dialog
	button_list =
	{
		// TODO
	};

	init_configs();

	// Define border values
	thumb_max = 255;
	trigger_min = 0;
	trigger_max = 255;

	// Set capabilities
	b_has_config = true;
	b_has_rumble = true;
	b_has_motion = true;
	b_has_deadzones = true;
	b_has_led = true;
	b_has_rgb = true;
	b_has_player_led = false;
	b_has_battery = true;
	b_has_battery_led = false;
	b_has_pressure_intensity_button = false;
	b_has_orientation = true;

	m_name_string = "OpenXR #";
	m_max_devices = 4; // CELL_GEM_MAX_NUM

	m_trigger_threshold = trigger_max / 2;
	m_thumb_threshold   = thumb_max / 2;
}

openxr_handler::~openxr_handler()
{
}

void openxr_handler::init_config(cfg_pad* cfg)
{
	if (!cfg) return;

	// Set default button mapping
	//cfg->ls_left.def  = ::at32(button_list, ps_move_key_codes::none);
	//cfg->ls_down.def  = ::at32(button_list, ps_move_key_codes::none);
	//cfg->ls_right.def = ::at32(button_list, ps_move_key_codes::none);
	//cfg->ls_up.def    = ::at32(button_list, ps_move_key_codes::none);
	//cfg->rs_left.def  = ::at32(button_list, ps_move_key_codes::none);
	//cfg->rs_down.def  = ::at32(button_list, ps_move_key_codes::none);
	//cfg->rs_right.def = ::at32(button_list, ps_move_key_codes::none);
	//cfg->rs_up.def    = ::at32(button_list, ps_move_key_codes::none);
	//cfg->start.def    = ::at32(button_list, ps_move_key_codes::start);
	//cfg->select.def   = ::at32(button_list, ps_move_key_codes::select);
	//cfg->ps.def       = cfg_pad::make_button_string(button_list, {{ps_move_key_codes::ps}, {ps_move_key_codes::start, ps_move_key_codes::select}});
	//cfg->square.def   = ::at32(button_list, ps_move_key_codes::square);
	//cfg->cross.def    = ::at32(button_list, ps_move_key_codes::cross);
	//cfg->circle.def   = ::at32(button_list, ps_move_key_codes::circle);
	//cfg->triangle.def = ::at32(button_list, ps_move_key_codes::triangle);
	//cfg->left.def     = ::at32(button_list, ps_move_key_codes::none);
	//cfg->down.def     = ::at32(button_list, ps_move_key_codes::none);
	//cfg->right.def    = ::at32(button_list, ps_move_key_codes::none);
	//cfg->up.def       = ::at32(button_list, ps_move_key_codes::none);
	//cfg->r1.def       = ::at32(button_list, ps_move_key_codes::move);
	//cfg->r2.def       = ::at32(button_list, ps_move_key_codes::t);
	//cfg->r3.def       = ::at32(button_list, ps_move_key_codes::none);
	//cfg->l1.def       = ::at32(button_list, ps_move_key_codes::none);
	//cfg->l2.def       = ::at32(button_list, ps_move_key_codes::none);
	//cfg->l3.def       = ::at32(button_list, ps_move_key_codes::none);

	//cfg->orientation_reset_button.def = ::at32(button_list, ps_move_key_codes::none);

	// Set default misc variables
	cfg->lstickdeadzone.def    = 40; // between 0 and 255
	cfg->rstickdeadzone.def    = 40; // between 0 and 255
	cfg->ltriggerthreshold.def = 0;  // between 0 and 255
	cfg->rtriggerthreshold.def = 0;  // between 0 and 255

	// We have to enable orientation by default
	cfg->orientation_enabled.def = true;

	// apply defaults
	cfg->from_default();
}

std::vector<pad_list_entry> openxr_handler::list_devices()
{
	if (!openxr_instance::is_openxr_available())
	{
		return {};
	}

	return {};
}

std::shared_ptr<PadDevice> openxr_handler::get_device(const std::string& device)
{
	return {};
}

void openxr_handler::SetPadData(const std::string& padId, u8 player_id, u8 large_motor, u8 small_motor, s32 r, s32 g, s32 b, bool /*player_led*/, bool /*battery_led*/, u32 /*battery_led_brightness*/)
{
}

u32 openxr_handler::get_battery_level(const std::string& padId)
{
	return 0;
}

bool openxr_handler::get_is_left_trigger(const std::shared_ptr<PadDevice>& device, u32 keyCode)
{
	return false;
}

bool openxr_handler::get_is_right_trigger(const std::shared_ptr<PadDevice>& device, u32 keyCode)
{
	return false;
}

bool openxr_handler::get_is_left_stick(const std::shared_ptr<PadDevice>& device, u32 keyCode)
{
	return false;
}

bool openxr_handler::get_is_right_stick(const std::shared_ptr<PadDevice>& device, u32 keyCode)
{
	return false;
}

PadHandlerBase::connection openxr_handler::update_connection(const std::shared_ptr<PadDevice>& device)
{
	return PadHandlerBase::connection::disconnected;
}

void openxr_handler::get_extended_info(const pad_ensemble& binding)
{
}

void openxr_handler::apply_pad_data(const pad_ensemble& binding)
{
}

std::unordered_map<u32, u16> openxr_handler::get_button_values(const std::shared_ptr<PadDevice>& device)
{
	return {};
}

pad_preview_values openxr_handler::get_preview_values(const std::unordered_map<u32, u16>& data, const std::vector<std::string>& buttons)
{
	return {};
}
