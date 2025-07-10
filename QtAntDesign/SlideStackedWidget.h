﻿#pragma once

#include <QStackedWidget>
#include <QPropertyAnimation>

class SlideStackedWidget : public QStackedWidget
{
	Q_OBJECT

public:
	enum SlideDirection {
		LeftToRight,
		RightToLeft
	};
	enum AnimationStyle {
		Standard,
		InOutCubic,
		Custom,
		Win11 // 新增
	};
	SlideStackedWidget(QWidget* parent = nullptr);
	~SlideStackedWidget();

	// 防误触
	bool isAnimationRunning() { return isAnimating; };
	// 左右滑动切换到下个页面
	void slideToPage(QWidget* nextWidget, SlideDirection direction = RightToLeft, int duration = 300,
		AnimationStyle style = Custom, std::function<void()> onFinished = nullptr);
	// 从下到上
	void slideFromBottomToTop(QWidget* nextWidget, int duration, std::function<void()> onFinished);
private:
	QEasingCurve getIOSEasingCurve(AnimationStyle style);

	bool isAnimating = false;
};
