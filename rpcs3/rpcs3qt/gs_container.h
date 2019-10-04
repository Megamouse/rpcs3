#pragma once

#include <QEvent>
#include <QWidget>

class gs_container : public QWidget
{
public:
	gs_container(QWidget* parent = nullptr);

protected:
	bool event(QEvent* ev) override;
};
