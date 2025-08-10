﻿#pragma once

#include <QObject>
#include <QColor>
#include <QFont>
#include "TransparentMask.h"
#include "MaskWidget.h"

struct Theme
{
	// 样式颜色
	QColor primaryColor;
	QColor primaryHoverColor;
	QColor textColor;
	QColor backgroundColor;
	QColor borderColor;
	QColor widgetBgColor;
	QColor widgetHoverBgColor;
	QColor widgetSelectedBgColor;
	QColor shadowColor;
	// 单选按钮相关颜色
	QColor borderColorHover;
	QColor disabledColor;
	// 全局消息相关颜色
	QColor msgTextColor;
	QColor msgBgColor;
	// 滑动条相关颜色
	QColor slideColor;
	QColor slideBgColor;
	QColor ringColor;
	QColor ringhoveredColor;
	// 开关按钮滑块颜色
	QColor toggleButtonColor;
	QColor toggleButtonBgColor;
	QColor noCheckedColor;
	// 提示框相关颜色
	QColor toolTipBgColor;
	QColor toolTipTextColor;
	// 用户信息气泡颜色
	QColor userBubbleBgColor;
	// 自定义图标按钮颜色
	QColor toolBtnBgColor;
	// 进度条相关颜色
	QColor progressBarBgColor;
	// 骨架屏相关颜色
	QColor skeletonBgColor;
	// tab相关颜色
	QColor tagTextColor;
	// line edit 颜色
	QColor lineEditBorderColor;
	QColor placeholderColor;
	// 多选按钮相关颜色
	QColor checkBoxBgColor;
	QColor checkBoxBorderEnableColor;
	QColor checkBoxBorderDisableColor;
	QColor checkBoxTextColor;
	QColor checkBoxTextDisableColor;
	// 下拉弹出框相关颜色
	QColor popupBgColor;
	QColor popupBorderColor;
	QColor popupScrollBarColor;
	QColor popupItemBgColor;
	QColor popupTextColor;
	// 滚动区相关颜色
	QColor scrollAreaHandleColor;
	QColor scrollAreaHoverColor;
	// 列表相关颜色
	QColor listTextColor;
	QColor listItemDescColor;
	// 表格相关颜色
	QColor tableTextColor;
	// 标签选项卡相关颜色
	QColor tabBarBgColor;
	QColor tabTextColor;
	QColor tabHoverColor;
	QColor onTabBtnHoverColor;
	QColor tabContainerColor;
	// 通知框背景色
	QColor notifBgColor;
	// 渐变颜色
	QColor vipGradientStartColor;
	QColor vipGradientMidColor;
	QColor vipGradientEndColor;
};

class DesignSystem : public QObject
{
	Q_OBJECT

public:
	enum ThemeMode { Light, Dark };
	enum TextColorMode { Black, White, DarkGray };

	static DesignSystem* instance()
	{
		if (!m_instance)
		{
			m_instance = new DesignSystem();
		}
		return m_instance;
	}

	// 当前主题访问
	const Theme& currentTheme() const;

	// 获取主窗口指针
	void setMainWindow(QWidget* mainWindow) { m_mainWindow = mainWindow; }
	QWidget* getMainWindow() const { return m_mainWindow; }
	int dialogWidth() const
	{
		if (!m_mainWindow)
			return 0;
		return static_cast<int>(m_mainWindow->width() * 0.42);
	}

	int dialogHeight() const
	{
		if (!m_mainWindow)
			return 0;
		return static_cast<int>(m_mainWindow->height() * 0.43);
	}

	// 快捷获取颜色等
	QColor primaryColor() const;
	QColor textColor() const;
	QColor backgroundColor() const;
	QColor widgetBgColor() const;
	QColor widgetHoverBgColor() const;
	QColor borderColor() const;
	QColor borderColorHover() const;
	QColor disabledColor() const;

	// 设置主题
	void setThemeMode(ThemeMode mode);
	ThemeMode themeMode() const;
	void switchTheme();
	QIcon& setThemeIcon();

	// 遮罩相关
	void setTransparentMask(TransparentMask* tpMask);
	TransparentMask* getTransparentMask();
	void setDarkMask(MaskWidget* mask);
	MaskWidget* getDarkMask();

	// 窗口操作按钮
	QIcon& btnMinIcon();
	QIcon& btnMaxIcon();
	QIcon& btnCloseIcon();
	QIcon& btnRestoreIcon();

	// 导航栏图标
	QString& btnHomeIconPath();
	QString& btnSettingsIconPath();
	QString& btnAboutIconPath();
	QString& btnHomeActiveIconPath();
	QString& btnSettingsActiveIconPath();
	QString& btnAboutActiveIconPath();

	// 前进后退图标
	QString& prevBtnIcon();
	QString& nextBtnIcon();
	QString& prevBtnDisableIcon();
	QString& nextBtnDisableIcon();
signals:
	void themeChanged();

private:
	DesignSystem();
	DesignSystem(const DesignSystem&) = delete;
	DesignSystem& operator=(const DesignSystem&) = delete;

	void loadThemes();

	static DesignSystem* m_instance;
	Theme m_lightTheme;
	Theme m_darkTheme;
	Theme m_currentTheme;
	ThemeMode m_mode;
	QWidget* m_mainWindow = nullptr;		// 用于获取主窗口
	TransparentMask* m_tpMask = nullptr;	// 全局透明遮罩
	MaskWidget* m_darkMask = nullptr;		// 全局深色遮罩
	QIcon m_themeIcon;
	QIcon btnMin;
	QIcon btnMax;
	QIcon btnClose;
	QIcon btnRestore;
	QString btnHome;
	QString btnHomeActive;
	QString btnSettings;
	QString btnSettingsActive;
	QString btnAbout;
	QString btnAboutActive;
	QString prevBtn;
	QString nextBtn;
	QString prevDisBtn;
	QString nextDisBtn;
};
