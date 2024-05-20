#pragma once

#include "util/types.hpp"
#include "Emu/Io/MouseHandler.h"

#include <QWindow>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTabletEvent>

namespace cfg
{
	class string;
}

class basic_mouse_handler final : public MouseHandlerBase, public QObject
{
	using MouseHandlerBase::MouseHandlerBase;

public:
	void Init(const u32 max_connect) override;

	void SetTargetWindow(QWindow* target);
	void MouseButtonDown(QMouseEvent* event);
	void MouseButtonUp(QMouseEvent* event);
	void MouseScroll(QWheelEvent* event);
	void MouseMove(QMouseEvent* event);

	void TabletEvent(QTabletEvent* event);

	bool eventFilter(QObject* obj, QEvent* ev) override;
private:
	QWindow* m_target = nullptr;
	bool get_mouse_lock_state() const;
	static int get_mouse_button(const cfg::string& button);

	std::map<u8, int> m_buttons;
};
