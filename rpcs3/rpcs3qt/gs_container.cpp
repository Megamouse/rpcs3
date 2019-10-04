#include "gs_container.h"
#include "gs_frame.h"

gs_container::gs_container(QWidget* parent)
	: QWidget(parent)
{
}

bool gs_container::event(QEvent* ev)
{
	if (ev->type() == QEvent::Close)
	{
		const auto frame = findChild<gs_frame*>();
		if (frame)
		{
			frame->try_to_close();
		}
	}
	return QWidget::event(ev);
}
