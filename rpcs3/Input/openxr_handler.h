#pragma once

#include "Emu/Io/PadHandler.h"

class openxr_handler : public PadHandlerBase
{
public:
	openxr_handler();
	virtual ~openxr_handler();

	std::vector<pad_list_entry> list_devices() override;
	void SetPadData(const std::string& padId, u8 player_id, u8 large_motor, u8 small_motor, s32 r, s32 g, s32 b, bool player_led, bool battery_led, u32 battery_led_brightness) override;
	u32 get_battery_level(const std::string& padId) override;
	void init_config(cfg_pad* cfg) override;

private:
	std::shared_ptr<PadDevice> get_device(const std::string& device) override;
	bool get_is_left_trigger(const std::shared_ptr<PadDevice>& device, u32 keyCode) override;
	bool get_is_right_trigger(const std::shared_ptr<PadDevice>& device, u32 keyCode) override;
	bool get_is_left_stick(const std::shared_ptr<PadDevice>& device, u32 keyCode) override;
	bool get_is_right_stick(const std::shared_ptr<PadDevice>& device, u32 keyCode) override;
	PadHandlerBase::connection update_connection(const std::shared_ptr<PadDevice>& device) override;
	void get_extended_info(const pad_ensemble& binding) override;
	void apply_pad_data(const pad_ensemble& binding) override;
	std::unordered_map<u32, u16> get_button_values(const std::shared_ptr<PadDevice>& device) override;
	pad_preview_values get_preview_values(const std::unordered_map<u32, u16>& data, const std::vector<std::string>& buttons) override;
};
