#include "shortcut_handler.h"
#include "Emu/System.h"
#include "Utilities/Log.h"

LOG_CHANNEL(shortcut_log, "Shortcuts");

shortcut_handler::shortcut_handler(gui::shortcuts::shortcut_handler_id handler_id, QWidget* parent, const std::shared_ptr<gui_settings>& gui_settings)
	: QWidget(parent)
{
	shortcut_settings sc_settings;

	// Initialize shortcuts
	QMap<gui::shortcuts::shortcut, const char*> shortcut_handlers;

	for (const auto& [shortcut_key, shortcut] : sc_settings.shortcut_map)
	{
		const auto key_sequence = sc_settings.get_key_sequence(shortcut, gui_settings);
		const bool is_global = handler_id == gui::shortcuts::shortcut_handler_id::global;

		// Skip shortcuts that weren't meant for this handler
		if (!is_global && handler_id != shortcut.id)
		{
			continue;
		}

		QShortcut* shortcut = new QShortcut(key_sequence, this);

		if (!is_global)
		{
			//shortcut->setContext(Qt::ShortcutContext::WidgetWithChildrenShortcut);
		}

		connect(shortcut, &QShortcut::activated, this, [this, shortcut_key, key_sequence]()
		{
			shortcut_log.fatal("activated: %d %s", static_cast<int>(shortcut_key), key_sequence.toString().toStdString());
			handle_shortcut(shortcut_key, key_sequence);
		});
		//connect(shortcut, &QShortcut::activatedAmbiguously, this, [this, shortcut_key, key_sequence]()
		//{
		//	// TODO: do not allow same shortcuts and remove this connect
		//	// activatedAmbiguously will trigger if you have the same key sequence for several shortcuts
		//	shortcut_log.fatal("activatedAmbiguously: %d %s", static_cast<int>(shortcut_key), key_sequence.toString().toStdString());
		//	handle_shortcut(shortcut_key, key_sequence);
		//});
	}
}

void shortcut_handler::handle_shortcut(gui::shortcuts::shortcut shortcut_key, const QKeySequence& key_sequence)
{
	shortcut_log.fatal("Shortcut pressed: %d %s", static_cast<int>(shortcut_key), key_sequence.toString().toStdString());

	switch (shortcut_key)
	{
	case gui::shortcuts::shortcut::shortcut_pause:
	{
		if (Emu.IsRunning())
			Emu.Pause();
		return;
	}
	case gui::shortcuts::shortcut::shortcut_restart:
	{
		if (Emu.IsPaused())
			Emu.Resume();
		else if (Emu.IsReady())
			Emu.Run(true);
		return;
	}
	case gui::shortcuts::shortcut::shortcut_start:
	{
		if (!Emu.GetBoot().empty())
			Emu.Restart();
		return;
	}
	case gui::shortcuts::shortcut::shortcut_stop:
	{
		if (!Emu.IsStopped())
			Emu.Stop();
		return;
	}
	default:
	{
		Q_EMIT shortcut_activated(shortcut_key, key_sequence);
		break;
	}
	}
}
