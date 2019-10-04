#pragma once

#include "gui_settings.h"
#include "shortcut_settings.h"

#include <QShortcut>
#include <QWidget>

class shortcut_handler : public QWidget
{
	Q_OBJECT

public:
	shortcut_handler(gui::shortcuts::shortcut_handler_id handler_id, QWidget* parent, const std::shared_ptr<gui_settings>& gui_settings);

Q_SIGNALS:
	void shortcut_activated(gui::shortcuts::shortcut shortcut_key, const QKeySequence& key_sequence);

private:
	void handle_shortcut(gui::shortcuts::shortcut shortcut_key, const QKeySequence& key_sequence);
};
