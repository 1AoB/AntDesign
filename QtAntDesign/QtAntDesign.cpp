﻿#include "QtAntDesign.h"
#include <qevent.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qscreen.h>
#include "StyleSheet.h"
#include "AntMessageManager.h"
#include "AntTooltipManager.h"
#include "DialogViewController.h"
#include "HomePage.h"
#include "SettingsPage.h"
#include "AboutPage.h"
#include <QToolButton>
#include <QLabel>
#include <QShowEvent>
#include "NotificationManager.h"
#include "DesignSystem.h"
#include "TransparentMask.h"
#include "MaskWidget.h"
#include "AntInput.h"
#include "ThemeSwitcher.h"

QtAntDesign::QtAntDesign(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setObjectName("QtAntDesign");
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_NoSystemBackground);
	QSize miniSize(1120, 725);
	setMinimumSize(miniSize);

	ui.main_widget->setStyleSheet(StyleSheet::mainQss(DesignSystem::instance()->backgroundColor()));

	// 获取主屏幕尺寸
	int w = 0, h = 0;
	QScreen* screen = QGuiApplication::primaryScreen();
	if (screen)
	{
		QSize screenSize = screen->availableSize();		// 可用屏幕大小，不包括任务栏
		w = int(screenSize.width() * 0.50);  // % 宽度
		h = int(screenSize.height() * 0.60); // % 高度
		if (w < miniSize.width() && h < miniSize.height())
		{
			w = miniSize.width();
			h = miniSize.height();
		}
		resize(w, h);
	}
	setContentsMargins(0, 0, 0, 0);

	// 初始化全局设计系统 必须写在最前面 因为它会设置主题和主窗口指针注册一些全局变量
	DesignSystem::instance()->setThemeMode(DesignSystem::Light);					// 默认亮主题
	DesignSystem::instance()->setMainWindow(this);									// 获取主窗口指针
	ThemeSwitcher* themeSwitcher = new ThemeSwitcher(this);
	themeSwitcher->setThemeColor();
	// 注册全局透明遮罩
	TransparentMask* tpMask = new TransparentMask(this);
	DesignSystem::instance()->setTransparentMask(tpMask);
	// 全局深色动画遮罩
	MaskWidget* darkMask = new MaskWidget(w, h, this);
	DesignSystem::instance()->setDarkMask(darkMask);

	// 任务栏 内容区域 导航栏 布局调整
	ui.navi_widget->setFixedWidth(m_naviWidth); // 希望的宽度
	ui.navi_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);	// 水平方向缩放策略固定
	ui.navi_widget->setStyleSheet(StyleSheet::naviQss(DesignSystem::instance()->widgetBgColor()));
	ui.titleBar->setStyleSheet(StyleSheet::titleBarQss(DesignSystem::instance()->backgroundColor()));
	ui.central->setStyleSheet(StyleSheet::centralQss());
	ui.titleBar->setFixedHeight(m_titleBarHeight);

	// 标题栏
	QHBoxLayout* titleLay = new QHBoxLayout(ui.titleBar);
	titleLay->setContentsMargins(20, 0, rightMargin, 0);
	titleLay->setSpacing(titleBarSpacing);
	QFont font;
	font.setPointSizeF(16);
	font.setBold(true);
	QLabel* title = new QLabel("TITLE", ui.titleBar);
	title->setFont(font);
	// 创建按钮
	btnMin = new QToolButton(ui.titleBar);
	btnMax = new QToolButton(ui.titleBar);
	btnClose = new QToolButton(ui.titleBar);
	// 设置图标
	btnMin->setIcon(DesignSystem::instance()->btnMinIcon());
	btnMax->setIcon(DesignSystem::instance()->btnMaxIcon());
	btnClose->setIcon(DesignSystem::instance()->btnCloseIcon());
	// 设置QSS
	btnMin->setStyleSheet(StyleSheet::toolBtnQss());
	btnMax->setStyleSheet(StyleSheet::toolBtnQss());
	btnClose->setStyleSheet(StyleSheet::toolBtnQss());
	// 设置固定大小（根据图标适当调整）
	btnMin->setFixedSize(32, 32);
	btnMax->setFixedSize(32, 32);
	btnClose->setFixedSize(34, 34);
	int extraWidth = 100;	// 额外的间距宽度: 是控件间距以及标题栏两端的间距 自己根据标题栏所有控件的宽度调整
	m_widgetTotalWidth = btnMin->width() + btnMax->width() + btnClose->width();

	// 搜索栏
	QStringList listItems = {
	"华为", "谷歌", "微软", "亚马逊", "特斯拉", "苹果",
	"英伟达", "Meta（脸书）", "字节跳动", "腾讯", "阿里巴巴",
	"百度",  "小米", "三星", "英特尔",
	"高通", "索尼", "拼多多", "OpenAI", "美团"
	};
	AntInput* antInput = new AntInput(300, listItems, ui.titleBar);
	antInput->setFixedWidth(254);
	antInput->setFixedHeight(46);
	antInput->setPlaceholderText("搜索内容");

	// 标题栏右侧所有控件的长宽转为物理像素后在native事件中限制标题栏的范围
	qreal dpiScale = QApplication::primaryScreen()->devicePixelRatio();
	// 计算控件总宽度（逻辑像素） 右侧4个控件3个间隔
	int widgetTotalWidth = rightMargin + btnMin->width() + btnMax->width() + btnClose->width() +
		antInput->width() + 3 * titleBarSpacing;
	// 转换为物理像素
	m_widgetTotalWidthPhysicalPixels = static_cast<int>(widgetTotalWidth * dpiScale);
	// 同理转换标题栏左侧
	m_titleLeftTotalWidthPhysicalPixels = static_cast<int>(m_naviWidth * dpiScale);
	// 同理转换标题栏高度
	m_titleBarHeightPhysicalPixels = static_cast<int>(m_titleBarHeight * dpiScale);

	// 将标题和按钮添加到布局
	titleLay->addWidget(title);
	titleLay->addStretch();
	titleLay->addWidget(antInput);
	titleLay->addWidget(btnMin);
	titleLay->addWidget(btnMax);
	titleLay->addWidget(btnClose);
	totalSpacingWidth = 3 * titleBarSpacing;	// 标题栏右侧4个控件中间3个间隔

	// 导航栏添加控件
	QVBoxLayout* naviLay = new QVBoxLayout(ui.navi_widget);;
	ui.navi_widget->layout()->setContentsMargins(0, 0, 0, 0);
	CircularAvatar* avatar = new CircularAvatar(QSize(42, 42), ":/Imgs/noLogin.svg", ":/Imgs/github.svg", ui.navi_widget);
	// 添加页面
	QVBoxLayout* contentLay = new QVBoxLayout(ui.central);
	contentLay->setContentsMargins(0, 0, 0, 0);
	contentLay->setSpacing(0);
	SlideStackedWidget* stackedWidget = new SlideStackedWidget(ui.central);
	contentLay->addWidget(stackedWidget);
	HomePage* homePage = new HomePage(stackedWidget);
	SettingsPage* settingsPage = new SettingsPage(stackedWidget);
	AboutPage* aboutPage = new AboutPage(stackedWidget);
	stackedWidget->addWidget(homePage);
	stackedWidget->addWidget(settingsPage);
	stackedWidget->addWidget(aboutPage);
	stackedWidget->setCurrentIndex(0);				// 默认显示首页
	// 导航按钮
	const int naviWidth = ui.navi_widget->width();  // 动态获取导航栏宽度
	const double iconSizeRatio = 0.56;				// 图标占按钮的比例
	const int buttonSize = naviWidth;
	const int iconSize = static_cast<int>(buttonSize * iconSizeRatio);
	CustomToolButton* btnHome = new CustomToolButton(QSize(iconSize, iconSize), ui.navi_widget);
	CustomToolButton* btnSettings = new CustomToolButton(QSize(iconSize, iconSize), ui.navi_widget);
	CustomToolButton* btnAbout = new CustomToolButton(QSize(iconSize, iconSize), ui.navi_widget);
	// 设置导航按钮样式
	auto* ins = DesignSystem::instance();
	buttonInfos = {
	{btnHome, ins->btnHomeIconPath(), ins->btnHomeActiveIconPath(), homePage},
	{btnSettings,ins->btnSettingsIconPath() ,ins->btnSettingsActiveIconPath() , settingsPage},
	{btnAbout,ins->btnAboutIconPath() , ins->btnAboutActiveIconPath(), aboutPage}
	};
	// 设置统一样式和连接信号
	buttonInfos[stackedWidget->currentIndex()].button->setBtnChecked(true); // 设置当前页面按钮为选中状态
	for (const ButtonInfo& info : buttonInfos)
	{
		CustomToolButton* btn = info.button;
		btn->setSvgIcons(info.normalIcon, info.activeIcon);
		btn->setFixedSize(QSize(naviWidth - 4, naviWidth - 4));
		// 连接信号，捕获图标路径
		connect(btn, &QToolButton::clicked, [btn, stackedWidget, this]()
			{
				// 禁用所有按钮，防止视觉反馈+误点
				for (ButtonInfo& infos : buttonInfos)
					infos.button->setEnabled(false);

				if (stackedWidget->isAnimationRunning()) return;

				// 更新UI
				QWidget* nextPage = nullptr;
				btn->setBtnChecked(true);
				for (ButtonInfo& infos : buttonInfos)
				{
					if (infos.button == btn)
						nextPage = infos.page;
					else
						infos.button->setBtnChecked(false);
				}

				// 页面切换 + 结束后恢复按钮
				stackedWidget->slideFromBottomToTop(nextPage, 250, [this]()
					{
						for (ButtonInfo& infos : buttonInfos)
							infos.button->setEnabled(true);
					});
			});
	}
	// 主题切换按钮
	QPushButton* themeBtn = new QPushButton(ui.navi_widget);
	themeBtn->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
	themeBtn->setFixedSize(naviWidth * 0.38, naviWidth * 0.38);
	themeBtn->setIcon(DesignSystem::instance()->setThemeIcon());
	themeBtn->setIconSize(themeBtn->size());
	connect(themeBtn, &QPushButton::clicked, this, [this, themeSwitcher, themeBtn]()
		{
			// 获取按钮中心点的全局坐标
			themeSwitcher->startSwitchTheme(this->grab(), themeBtn, themeBtn->mapToGlobal(themeBtn->rect().center()));
		});
	connect(this, &QtAntDesign::resized, themeSwitcher, &ThemeSwitcher::resizeByMainWindow);

	//  布局
	naviLay->addSpacing(28);
	naviLay->addWidget(avatar, 0, Qt::AlignHCenter);
	naviLay->addSpacing(16);
	naviLay->addWidget(btnHome, 0, Qt::AlignHCenter);
	naviLay->addWidget(btnSettings, 0, Qt::AlignHCenter);
	naviLay->addWidget(btnAbout, 0, Qt::AlignHCenter);
	naviLay->addStretch();
	naviLay->addWidget(themeBtn, 0, Qt::AlignHCenter);
	naviLay->addSpacing(70);

	// 初始化全局管理器
	AntMessageManager::instance();	// 全局消息
	AntTooltipManager::instance();	// 全局提示

	// 初始化通知管理器
	NotificationManager::instance()->getMainWindow(ui.main_widget);

	// 调整消息框位置
	connect(this, &QtAntDesign::resized, this, [=](int w, int h)
		{
			NotificationManager::instance()->relayoutNotifications(w, h);
		});

	// 对话框
	DialogViewController* mDialog = new DialogViewController(avatar->loginState(), this);	// 实际登录状态要服务器给予
	avatar->addDialog(mDialog);
	connect(mDialog, &DialogViewController::successLogin, avatar, &CircularAvatar::allowLogin);

	// 信号连接
	connect(btnMax, &QToolButton::clicked, this, [this]()
		{
			// 每次使用时重新获取窗口句柄，可以确保你拿到的是当前最新、有效的窗口句柄。避免使用缓存失效的句柄造成崩溃。
			HWND hwnd = reinterpret_cast<HWND>(winId());

			// 如果当前最大化，点击恢复；否则最大化
			WINDOWPLACEMENT wp;
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hwnd, &wp);

			if (wp.showCmd == SW_MAXIMIZE)
			{
				ShowWindow(hwnd, SW_RESTORE);
			}
			else
			{
				ShowWindow(hwnd, SW_MAXIMIZE);
			}
		});

	connect(btnMin, &QToolButton::clicked, this, [this]()
		{
			HWND hwnd = reinterpret_cast<HWND>(winId());
			ShowWindow(hwnd, SW_MINIMIZE);
		});

	connect(btnClose, &QToolButton::clicked, this, [this, w, h]()
		{
			emit showStandardDialog("TITLE", "是否关闭应用程序?");
		});

	connect(this, &QtAntDesign::showStandardDialog, mDialog, &DialogViewController::buildStandardDialog);

	// 主页的信号连接
	connect(this, &QtAntDesign::resized, homePage, &HomePage::resized);
	connect(this, &QtAntDesign::resized, this, [this](int w, int h)
		{
			DesignSystem::instance()->getDarkMask()->resize(w, h);
		});
	connect(this, &QtAntDesign::windowMoved, homePage, &HomePage::windowMoved);

	// 主题切换
	connect(ins, &DesignSystem::themeChanged, this, [=]()
		{
			ui.main_widget->setStyleSheet(StyleSheet::mainQss(DesignSystem::instance()->backgroundColor()));
			ui.navi_widget->setStyleSheet(StyleSheet::naviQss(DesignSystem::instance()->widgetBgColor()));
			ui.titleBar->setStyleSheet(StyleSheet::titleBarQss(DesignSystem::instance()->backgroundColor()));
			btnMin->setIcon(DesignSystem::instance()->btnMinIcon());
			btnMax->setIcon(DesignSystem::instance()->btnMaxIcon());
			btnClose->setIcon(DesignSystem::instance()->btnCloseIcon());
			buttonInfos = {
				{btnHome, ins->btnHomeIconPath(), ins->btnHomeActiveIconPath(), homePage},
				{btnSettings,ins->btnSettingsIconPath() ,ins->btnSettingsActiveIconPath() , settingsPage},
				{btnAbout,ins->btnAboutIconPath() , ins->btnAboutActiveIconPath(), aboutPage}
			};
			for (const ButtonInfo& info : buttonInfos)
			{
				CustomToolButton* btn = info.button;
				btn->setSvgIcons(info.normalIcon, info.activeIcon);
			}
		});
}

QtAntDesign::~QtAntDesign()
{
}

void QtAntDesign::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);

	// 调整对话框尺寸
	emit resized(width(), height());

	qDebug() << "width()" << width() << "height()" << height();
}

bool QtAntDesign::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
#ifdef Q_OS_WIN
	MSG* msg = static_cast<MSG*>(message);

	switch (msg->message) {
	case WM_NCCALCSIZE: {
		// 去掉非客户区，让客户区占满整个窗口（无边框）
		if (msg->wParam) {
			*result = 0;
			return true;
		}
		break;
	}
	case WM_NCLBUTTONDBLCLK: {
		// 如果存在遮罩则响应遮罩，禁止系统事件（双击最大化）
		if (DesignSystem::instance()->getTransparentMask()->isVisible() ||
			DesignSystem::instance()->getDarkMask()->isVisible())
		{
			*result = 0;  // 阻止系统响应双击
			return true;
		}
		break;
	}
	case WM_NCHITTEST: {
		if (DesignSystem::instance()->getTransparentMask()->isVisible() ||
			DesignSystem::instance()->getDarkMask()->isVisible())
		{
			*result = HTCLIENT;  // 阻止系统响应拖拽缩放
			return true;
		}

		// 处理拖拽和缩放区域
		const LONG borderWidth = 8; // 拖拽缩放边框宽度

		// 返回的是物理像素
		RECT winRect;
		GetWindowRect(HWND(winId()), &winRect);

		// 获取鼠标全局坐标物理像素
		const LONG x = GET_X_LPARAM(msg->lParam);
		const LONG y = GET_Y_LPARAM(msg->lParam);

		// 允许缩放的条件（根据窗口最小最大宽高判断）
		const bool canResizeWidth = minimumWidth() != maximumWidth();
		const bool canResizeHeight = minimumHeight() != maximumHeight();

		// 左上角
		if (canResizeWidth && canResizeHeight &&
			x >= winRect.left && x < winRect.left + borderWidth &&
			y >= winRect.top && y < winRect.top + borderWidth) {
			*result = HTTOPLEFT;
			return true;
		}
		// 右上角
		if (canResizeWidth && canResizeHeight &&
			x >= winRect.right - borderWidth && x < winRect.right &&
			y >= winRect.top && y < winRect.top + borderWidth) {
			*result = HTTOPRIGHT;
			return true;
		}
		// 左下角
		if (canResizeWidth && canResizeHeight &&
			x >= winRect.left && x < winRect.left + borderWidth &&
			y >= winRect.bottom - borderWidth && y < winRect.bottom) {
			*result = HTBOTTOMLEFT;
			return true;
		}
		// 右下角
		if (canResizeWidth && canResizeHeight &&
			x >= winRect.right - borderWidth && x < winRect.right &&
			y >= winRect.bottom - borderWidth && y < winRect.bottom) {
			*result = HTBOTTOMRIGHT;
			return true;
		}
		// 左边
		if (canResizeWidth &&
			x >= winRect.left && x < winRect.left + borderWidth) {
			*result = HTLEFT;
			return true;
		}
		// 右边
		if (canResizeWidth &&
			x >= winRect.right - borderWidth && x < winRect.right) {
			*result = HTRIGHT;
			return true;
		}
		// 上边
		if (canResizeHeight &&
			y >= winRect.top && y < winRect.top + borderWidth) {
			*result = HTTOP;
			return true;
		}
		// 下边
		if (canResizeHeight &&
			y >= winRect.bottom - borderWidth && y < winRect.bottom) {
			*result = HTBOTTOM;
			return true;
		}
		// 设置标题栏拖动区域 只有该区域内才允许拖动窗口
		if (x > winRect.left + m_titleLeftTotalWidthPhysicalPixels && x < winRect.right - m_widgetTotalWidthPhysicalPixels
			&& y > winRect.top && y < winRect.top + m_titleBarHeightPhysicalPixels)
		{
			*result = HTCAPTION;
			return true;
		}
		// 其余地方交给默认处理
		break;
	}
	default:
		break;
	}
#endif // Q_OS_WIN
	return QWidget::nativeEvent(eventType, message, result);
}

void QtAntDesign::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);

#ifdef Q_OS_WIN
	m_hwnd = reinterpret_cast<HWND>(winId());
	if (!m_hwnd) return;

	LONG style = GetWindowLong(m_hwnd, GWL_STYLE);

	// 非客户区被隐藏了 但是他还有标题栏、有边框以及最大化最小化功能的标准窗口从而提供windows原生动画和交互
	style |= WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	SetWindowLong(m_hwnd, GWL_STYLE, style);

	DWORD cornerPref = 2;
	DwmSetWindowAttribute(m_hwnd, 33, &cornerPref, sizeof(cornerPref));
#endif // Q_OS_WIN
}

void QtAntDesign::moveEvent(QMoveEvent* event)
{
	QWidget::moveEvent(event);
	// 发送窗口左上角全局坐标
	emit windowMoved(this->mapToGlobal(QPoint(0, 0)));
}

void QtAntDesign::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::WindowStateChange)
	{
		QWindowStateChangeEvent* stateEvent = static_cast<QWindowStateChangeEvent*>(event);

		if (isMaximized())
		{
			btnMax->setIcon(DesignSystem::instance()->btnRestoreIcon());
			ui.main_widget->layout()->setContentsMargins(0, 0, 5, 0);
		}
		else if (stateEvent->oldState() & Qt::WindowMaximized)
		{
			btnMax->setIcon(DesignSystem::instance()->btnMaxIcon());
			ui.main_widget->layout()->setContentsMargins(0, 0, 0, 0);
		}
	}

	QWidget::changeEvent(event);
}