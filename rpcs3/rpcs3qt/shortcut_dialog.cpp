#include "shortcut_dialog.h"
#include "ui_shortcut_dialog.h"
#include "shortcut_settings.h"

#include <QDialogButtonBox>
#include <QKeySequenceEdit>
#include <QLabel>
#include <QHBoxLayout>

constexpr auto qstr = QString::fromStdString;

shortcut_dialog::shortcut_dialog(const std::shared_ptr<gui_settings> gui_settings, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::shortcut_dialog)
    , m_gui_settings(gui_settings)
{
	ui->setupUi(this);

	shortcut_settings sc_settings;

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &shortcut_dialog::handle_save);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &shortcut_dialog::reject);

	for (const auto& [shortcut_key, shortcut] : sc_settings.shortcut_map)
	{
		const auto key_sequence = sc_settings.get_key_sequence(shortcut, gui_settings);

		QKeySequenceEdit* key_sequence_edit = new QKeySequenceEdit;
		key_sequence_edit->setObjectName(shortcut.name);
		key_sequence_edit->setMinimumWidth(QLabel(shortcut.localized_name).sizeHint().width());
		key_sequence_edit->setKeySequence(key_sequence);

		m_values[shortcut.name] = key_sequence.toString();

		connect(key_sequence_edit, &QKeySequenceEdit::keySequenceChanged, this, &shortcut_dialog::handle_change);

		QHBoxLayout* shortcut_layout = new QHBoxLayout;
		shortcut_layout->addWidget(new QLabel(shortcut.localized_name));
		shortcut_layout->addWidget(key_sequence_edit);
		shortcut_layout->setStretch(0, 1);
		shortcut_layout->setStretch(1, 1);

		switch (shortcut.id)
		{
		case gui::shortcuts::shortcut_handler_id::game_window:
		{
			ui->game_window_layout->insertLayout(-1, shortcut_layout);
			break;
		}
		case gui::shortcuts::shortcut_handler_id::main_window:
		{
			ui->main_window_layout->insertLayout(-1, shortcut_layout);
			break;
		}
		case gui::shortcuts::shortcut_handler_id::global:
		default:
		{
			ui->global_layout->insertLayout(-1, shortcut_layout);
			break;
		}
		}
	}

	ui->main_window_layout->addStretch();
	ui->game_window_layout->addStretch();
	ui->global_layout->addStretch();

	const int min_width = std::max(
	{
		ui->main_window_group_box->sizeHint().width(),
		ui->game_window_group_box->sizeHint().width(),
		ui->global_group_box->sizeHint().width()
	});

	ui->main_window_group_box->setMinimumWidth(min_width);
	ui->game_window_group_box->setMinimumWidth(min_width);
	ui->global_group_box->setMinimumWidth(min_width);
}

shortcut_dialog::~shortcut_dialog()
{
	delete ui;
}

void shortcut_dialog::handle_save()
{
	shortcut_settings sc_settings;

	for (const auto& entry : m_values)
	{
		m_gui_settings->SetValue(sc_settings.get_shortcut_gui_save(entry.first), entry.second);
	}

	accept();
}

void shortcut_dialog::handle_change(const QKeySequence& keySequence)
{
	m_values[sender()->objectName()] = keySequence.toString();
}
