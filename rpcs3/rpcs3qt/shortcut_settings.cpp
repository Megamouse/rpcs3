#include "shortcut_settings.h"

using namespace gui::shortcuts;

shortcut_settings::shortcut_settings()
	: shortcut_map({
		{ shortcut::shortcut_start, shortcut_info{ "start", tr("Start"), "Ctrl+E", shortcut_handler_id::global } },
		{ shortcut::shortcut_stop, shortcut_info{ "stop", tr("Stop"), "Ctrl+S", shortcut_handler_id::global } },
		{ shortcut::shortcut_pause, shortcut_info{ "pause", tr("Pause"), "Ctrl+P", shortcut_handler_id::global } },
		{ shortcut::shortcut_restart, shortcut_info{ "restart", tr("Restart"), "Ctrl+R", shortcut_handler_id::global } },
		{ shortcut::shortcut_mw_toggle_fullscreen, shortcut_info{ "main_window_toggle_fullscreen", tr("Toggle Fullscreen"), "Alt+Return", shortcut_handler_id::main_window } },
		{ shortcut::shortcut_mw_exit_fullscreen, shortcut_info{ "main_window_exit_fullscreen", tr("Exit Fullscreen"), "Esc", shortcut_handler_id::main_window } },
		{ shortcut::shortcut_gw_toggle_fullscreen, shortcut_info{ "game_window_toggle_fullscreen", tr("Toggle Fullscreen"), "Alt+Return", shortcut_handler_id::main_window } },
		{ shortcut::shortcut_gw_exit_fullscreen, shortcut_info{ "game_window_exit_fullscreen", tr("Exit Fullscreen"), "Esc", shortcut_handler_id::game_window } },
		{ shortcut::shortcut_gw_log_mark, shortcut_info{ "game_window_log_mark", tr("Add Log Mark"), "Alt+L", shortcut_handler_id::game_window } },
		{ shortcut::shortcut_gw_screenshot, shortcut_info{ "game_window_screenshot", tr("Screenshot"), "F12", shortcut_handler_id::game_window } },
	})
{
}

shortcut_settings::~shortcut_settings()
{
}

gui_save shortcut_settings::get_shortcut_gui_save(const QString& shortcut_name)
{
	const auto result = std::find_if
	(
		shortcut_map.begin(),
		shortcut_map.end(),
		[shortcut_name](const std::pair<const gui::shortcuts::shortcut, const shortcut_info>& entry)
		{
			return entry.second.name == shortcut_name;
		}
	);

	if (result != shortcut_map.end())
	{
		return gui_save(gui::sc, result->second.name, result->second.key_sequence);
	}

	return gui_save();
}

QKeySequence shortcut_settings::get_key_sequence(const shortcut_info& entry, const std::shared_ptr<gui_settings>& gui_settings)
{
	const QString saved_value = gui_settings->GetValue(get_shortcut_gui_save(entry.name)).toString();

	QKeySequence key_sequence = QKeySequence::fromString(saved_value);

	if (key_sequence.isEmpty())
	{
		// Use the default shortcut if no shortcut was configured
		key_sequence = QKeySequence::fromString(entry.key_sequence);
	}

	return key_sequence;
}
