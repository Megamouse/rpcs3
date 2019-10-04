#pragma once

#include "gui_settings.h"

#include <QDialog>

namespace Ui
{
	class shortcut_dialog;
}

class shortcut_dialog : public QDialog
{
	Q_OBJECT

public:
	explicit shortcut_dialog(const std::shared_ptr<gui_settings> gui_settings, QWidget* parent = nullptr);
	~shortcut_dialog();

private:
	Ui::shortcut_dialog* ui;
	std::shared_ptr<gui_settings> m_gui_settings;
	std::map<QString, QString> m_values;

private Q_SLOTS:
	void handle_save();
	void handle_change(const QKeySequence& keySequence);
};
