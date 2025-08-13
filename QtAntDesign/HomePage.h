﻿#pragma once

#include <QWidget>
#include "FramelessVideoWindow.h"

class HomePage : public QWidget
{
	Q_OBJECT

public:
	HomePage(QWidget* parent);
	~HomePage();
protected:
	void showEvent(QShowEvent* event) override;

private:
	FramelessVideoWindow* videoWindow = nullptr;
};
