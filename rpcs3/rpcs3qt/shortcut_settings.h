#pragma once

#include "gui_settings.h"

#include <QKeySequence>

namespace gui
{
	namespace shortcuts
	{
		enum class shortcut_handler_id : int
		{
			global,
			game_window,
			main_window,
		};

		enum class shortcut : int
		{
			shortcut_start,
			shortcut_stop,
			shortcut_pause,
			shortcut_restart,

			shortcut_mw_toggle_fullscreen,
			shortcut_mw_exit_fullscreen,

			shortcut_gw_toggle_fullscreen,
			shortcut_gw_exit_fullscreen,
			shortcut_gw_log_mark,
			shortcut_gw_screenshot,

			shortcut_count
		};
	}
}

struct shortcut_info
{
	QString name;
	QString localized_name;
	QString key_sequence;
	gui::shortcuts::shortcut_handler_id id;
};

class shortcut_settings : public QObject
{
	Q_OBJECT

public:
	shortcut_settings();
	~shortcut_settings();

	const std::map<const gui::shortcuts::shortcut, const shortcut_info> shortcut_map;

	gui_save get_shortcut_gui_save(const QString& shortcut_name);

	QKeySequence get_key_sequence(const shortcut_info& entry, const std::shared_ptr<gui_settings>& gui_settings);
};
