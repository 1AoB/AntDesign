#include "FunctionPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include "AntToggleButton.h"
#include "SlideStackedWidget.h"
#include "MaterialProgressBar.h"
#include <QList>
#include <QTimer>
#include <QScrollArea>
#include <QRegularExpression>
#include <QApplication>
#include "MaterialSpinner.h"
#include "AntRadioButton.h"
#include "AntSlider.h"
#include "NoDataWidget.h"
#include "AnimatedNumber.h"
#include "AntButton.h"
#include "NotificationManager.h"
#include "AntNumberInput.h"
#include "AntDoubleNumberInput.h"
#include "AntComboBox.h"
#include "TagWidget.h"
#include "CardWidget.h"
#include "QrCodeWidget.h"
#include "FlowLayout.h"
#include "DrawerWidget.h"
#include "BadgeWidget.h"
#include "AntChatListView.h"
#include "AntCellWidget.h"
#include "StyleSheet.h"
#include "PaginationWidget.h"
#include "AntTreeView.h"

FunctionPage::FunctionPage(QWidget* parent)
	: QWidget(parent)
{
	setObjectName("FunctionPage");
	setFocusPolicy(Qt::ClickFocus);  // 设置焦点策略 点击空白处可以获取焦点

	// 标签页
	tabWidget = new MaterialTabWidget(this);
	tabWidget->getLayout()->setContentsMargins(15, 0, 5, 0);

	// 滚动区域
	scrollArea1 = new AntScrollArea(AntScrollArea::ScrollVertical, this);
	QWidget* w1 = new QWidget(this);
	scrollArea1->addWidget(w1);

	// 视图页
	initViewPage();

	// 流式布局
	QWidget* w3 = new QWidget(this);
	FlowLayout* flowLay = new FlowLayout(w3, 10, 6);
	QIcon svgIcon(":/Imgs/git.svg");
	for (int i = 0; i < 50; ++i)
	{
		QLabel* label = new QLabel(w3);
		label->setFixedSize(50, 50);
		label->setPixmap(svgIcon.pixmap(50, 50));
		flowLay->addWidget(label);
	}

	// 暂无数据
	NoDataWidget* noData = new NoDataWidget(this);

	// 添加标签项
	tabWidget->addTab(scrollArea1, QString::fromLocal8Bit("常用控件"));
	tabWidget->addTab(scrollArea2, QString::fromLocal8Bit("视图控件"));
	tabWidget->addTab(w3, QString::fromLocal8Bit("流式布局"));
	tabWidget->addTab(noData, QString::fromLocal8Bit("暂无数据"));

	// 主布局
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(tabWidget);

	// 页面内容布局
	QVBoxLayout* pageLay = new QVBoxLayout(w1);
	pageLay->setSpacing(14);
	pageLay->setContentsMargins(0, 0, 0, 0);

	// 轮播图
	QStringList imagePaths = {
	":/Imgs/nvidia.jpg",
	":/Imgs/huawei.jpg",
	":/Imgs/openai.jpg",
	":/Imgs/wechat.jpg",
	":/Imgs/grok.jpg",
	":/Imgs/varda.jpg",
	":/Imgs/google.jpg",
	":/Imgs/apple.jpg",
	":/Imgs/amd.jpg"
	};

	carousel = new CarouselWidget(QSize(1150, 330), 5, imagePaths, this);
	// 启用标签
	carousel->getCards()[0]->addTab(QString::fromLocal8Bit("新发布"), QColor(255, 50, 49));					// 鲜红，新品强调
	carousel->getCards()[1]->addTab(QString::fromLocal8Bit("限时折扣"), QColor(255, 136, 0));					// 橙色，促销活动
	carousel->getCards()[2]->addTab(QString::fromLocal8Bit("热销中"), QColor(255, 69, 0));					// 橘红，高热度
	carousel->getCards()[3]->addTab(QString::fromLocal8Bit("推荐"), QColor(0, 160, 233));					// 天蓝，系统推荐
	carousel->getCards()[4]->addTab(QString::fromLocal8Bit("精选"), QColor(92, 184, 92));					// 草绿，高质量精选
	carousel->getCards()[5]->addTab(QString::fromLocal8Bit("预售"), QColor(255, 102, 153));					// 粉色，预售状态
	carousel->getCards()[6]->addTab(QString::fromLocal8Bit("会员专享"), QColor(255, 215, 0));					// 金色，专属权益
	carousel->getCards()[7]->addTab(QString::fromLocal8Bit("限量发售"), QColor(153, 102, 255));				// 紫色，稀缺商品
	carousel->getCards()[8]->addTab(QString::fromLocal8Bit("免费试用"), QColor(135, 206, 250));				// 浅蓝色
	// 启用文本内容 启用url按钮
	carousel->getCards()[0]->setTextContent("英伟达RTX显卡（NVIDIA Graphics Cards）");
	carousel->getCards()[0]->enableUrlBtn(QUrl("https://www.nvidia.cn/geforce/"));

	carousel->getCards()[1]->setTextContent("华为麒麟芯片（HiSilicon Kirin SoCs）");
	carousel->getCards()[1]->enableUrlBtn(QUrl("https://www.huawei.com/cn/"));

	carousel->getCards()[2]->setTextContent("OpenAI 人工智能技术（OpenAI AI Technologies）");
	carousel->getCards()[2]->enableUrlBtn(QUrl("https://www.openai.com/"));

	carousel->getCards()[3]->setTextContent("微信社交平台（WeChat Social Platform）");
	carousel->getCards()[3]->enableUrlBtn(QUrl("https://www.wechat.com/"));

	carousel->getCards()[4]->setTextContent("Grok AI 助手（Grok AI Assistant）");
	carousel->getCards()[4]->enableUrlBtn(QUrl("https://www.grok.com/"));  // 如果没有官网请提供或改掉

	carousel->getCards()[5]->setTextContent("Varda 太空制造公司（Varda Space Industries）");
	carousel->getCards()[5]->enableUrlBtn(QUrl("https://www.varda.com/"));

	carousel->getCards()[6]->setTextContent("谷歌搜索与服务（Google Search & Services）");
	carousel->getCards()[6]->enableUrlBtn(QUrl("https://www.google.com/"));

	carousel->getCards()[7]->setTextContent("苹果M系列芯片（Apple Silicon M-Series）");
	carousel->getCards()[7]->enableUrlBtn(QUrl("https://www.apple.com/mac/"));

	carousel->getCards()[8]->setTextContent("AMD锐龙CPU（AMD Ryzen Series）");
	carousel->getCards()[8]->enableUrlBtn(QUrl("https://www.amd.com/zh/products/ryzen-processors"));

	QHBoxLayout* carouselLayout = new QHBoxLayout();
	carouselLayout->setSpacing(0);
	carouselLayout->setContentsMargins(0, 0, 0, 0);

	// 添加标题 + 开关按钮
	QHBoxLayout* row1Layout = new QHBoxLayout();
	row1Layout->setSpacing(20); // label 和按钮之间的间距

	QHBoxLayout* row2Layout = new QHBoxLayout();
	row2Layout->setSpacing(10);

	QStringList nameList;
	QList<QLabel*>labelList;
	nameList << QString::fromLocal8Bit("开关按钮") << QString::fromLocal8Bit("进度条") << QString::fromLocal8Bit("水平滑动条") << QString::fromLocal8Bit("统计数值");
	for (auto& str : nameList)
	{
		QLabel* nameLabel = new QLabel(str, this);
		nameLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
		nameLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		labelList.append(nameLabel);
	}
	// 开关按钮
	AntToggleButton* toggleBtn = new AntToggleButton(QSize(57, 26), this);
	toggleBtn->setShowText(true);

	// 进度条
	MaterialProgressBar* progress = new MaterialProgressBar(this);
	progress->setFixedHeight(10);
	// 启动测试模式
	progress->startTestPattern();
	// 圆形进度条
	MaterialSpinner* spinner = new MaterialSpinner(QSize(40, 40), 4, DesignSystem::instance()->primaryColor(), this);

	// 单选按钮
	AntRadioButton* radioBtn1 = new AntRadioButton(this);
	radioBtn1->setText("单选按钮1");
	AntRadioButton* radioBtn2 = new AntRadioButton(this);
	radioBtn2->setText("单选按钮2");

	// 水平滑动条
	AntSlider* slider = new AntSlider(0, 100, 30, this);
	slider->setFixedWidth(230);

	// 统计数值
	AnimatedNumber* animNum = new AnimatedNumber(this);
	animNum->setTextWidth(120);
	animNum->setFontSzie(16);
	animNum->animateTo(12345);

	// 骨架屏
	AntButton* skeletonDescBtn = new AntButton(QString::fromLocal8Bit("骨架屏启动"), 12, w1);
	skeletonDescBtn->setFixedSize(120, 50);
	QHBoxLayout* row3Layout = new QHBoxLayout();
	row3Layout->setSpacing(8);
	row3Layout->setContentsMargins(0, 0, 0, 16);

	QList<QIcon> icons = {
		QIcon(":/Imgs/undraw_book-lover_f1dq.svg"),
		QIcon(":/Imgs/undraw_developer-avatar_f6ac.svg"),
		QIcon(":/Imgs/undraw_loving-it_hspq.svg")
	};

	QList<QLabel*> iconLabels;

	for (const QIcon& icon : icons)
	{
		SkeletonWidget* skeleton = new SkeletonWidget(8, this);
		skeleton->setFixedSize(300, 200);
		row3Layout->addWidget(skeleton);
		skeletons.append(skeleton);  // 保存骨架指针

		QLabel* iconLabel = new QLabel(this);
		iconLabel->setAlignment(Qt::AlignCenter);
		iconLabel->setPixmap(icon.pixmap(350, 250));
		iconLabel->hide();  // 初始隐藏
		iconLabels.append(iconLabel);
		row3Layout->addWidget(iconLabel);
	}

	row3Layout->addStretch();

	connect(skeletonDescBtn, &AntButton::clicked, this, [this, skeletonDescBtn, iconLabels]()
		{
			skeletonDescBtn->setEnabled(false);  // 禁用按钮，防止重复点击
			// 启动骨架动画
			for (int i = 0; i < iconLabels.size(); ++i)
			{
				iconLabels[i]->hide();  // 隐藏真实图标
			}

			for (SkeletonWidget* skeleton : skeletons)
			{
				skeleton->startSkeleton();
			}

			// 延迟2秒后隐藏骨架，显示图标
			QTimer::singleShot(3000, this, [=]()
				{
					for (int i = 0; i < skeletons.size(); ++i)
					{
						skeletons[i]->stopSkeleton();   // 停止并隐藏骨架
					}

					for (int i = 0; i < iconLabels.size(); ++i)
					{
						iconLabels[i]->show();
					}
					skeletonDescBtn->setEnabled(true);
				});
		});

	// 任务通知
	QHBoxLayout* row4Layout = new QHBoxLayout();
	row4Layout->setSpacing(18);
	row4Layout->setContentsMargins(0, 0, 0, 0);

	AntButton* taskBtn = new AntButton(QString::fromLocal8Bit("任务通知"), 12, w1);
	taskBtn->setFixedSize(120, 50);
	connect(taskBtn, &AntButton::clicked, this, [this]()
		{
			NotificationManager::instance()->showNotification(QString::fromLocal8Bit("任务通知"));
		});

	// 数字输入框
	QLabel* spinBoxLabel1 = new QLabel(QString::fromLocal8Bit("数字输入框"), this);
	AntNumberInput* spinbox = new AntNumberInput(this);
	spinbox->setFixedSize(90, 46);
	spinbox->input()->setRange(0, 100);			// 调整数值范围
	spinbox->input()->setValue(10);				// 设置初始值
	// 小数数字输入框
	QLabel* spinBoxLabel2 = new QLabel(QString::fromLocal8Bit("小数数字输入框"), this);
	AntDoubleNumberInput* doubleSpinBox = new AntDoubleNumberInput(this);
	doubleSpinBox->setFixedSize(90, 46);
	doubleSpinBox->input()->setDecimals(1);					// 设置保留 1 位小数
	doubleSpinBox->input()->setSingleStep(1);				// 每次加/减
	doubleSpinBox->input()->setRange(0.0, 100.0);			// 范围设置
	doubleSpinBox->input()->setValue(0.0);					// 初始值设置

	QHBoxLayout* row5Layout = new QHBoxLayout();
	row5Layout->setSpacing(18);
	row5Layout->setContentsMargins(0, 0, 0, 0);

	// 单层级下拉框
	QLabel* comboLabel1 = new QLabel(QString::fromLocal8Bit("下拉框"), this);
	QStringList topItems1 = { QString::fromLocal8Bit("水果"), QString::fromLocal8Bit("蔬菜"), QString::fromLocal8Bit("饮料"), QString::fromLocal8Bit("汤"), QString::fromLocal8Bit("零食"), QString::fromLocal8Bit("烘焙"), QString::fromLocal8Bit("速食") };
	AntComboBox* combo1 = new AntComboBox(QString::fromLocal8Bit("请选择"), topItems1, this);
	combo1->setFixedSize(185, 48);
	// 多层级下拉框
	QLabel* comboLabel2 = new QLabel(QString::fromLocal8Bit("多层级下拉框"), this);
	QStringList topItems2 = { QString::fromLocal8Bit("水果"), QString::fromLocal8Bit("蔬菜"), QString::fromLocal8Bit("饮料") };
	QMap<QString, QStringList> subItemMap = {
		{ QString::fromLocal8Bit("水果"), {QString::fromLocal8Bit("苹果"), QString::fromLocal8Bit("香蕉"), QString::fromLocal8Bit("西瓜")} },
		{ QString::fromLocal8Bit("蔬菜"), {QString::fromLocal8Bit("白菜"), QString::fromLocal8Bit("萝卜"), QString::fromLocal8Bit("西红柿")} },
		{ QString::fromLocal8Bit("饮料"), {QString::fromLocal8Bit("可乐"), QString::fromLocal8Bit("雪碧"), QString::fromLocal8Bit("果汁")} }
	};
	AntComboBox* combo2 = new AntComboBox(QString::fromLocal8Bit("请选择"), topItems2, this, 200, true, subItemMap);
	combo2->setFixedSize(185, 48);

	// 让下拉框遮罩跟随页面大小变化
	connect(this, &FunctionPage::resized, this, [=](int w, int h)
		{
			combo1->getMask()->resize(w, h);
			combo2->getMask()->resize(w, h);
		});

	// 下拉框子菜单定位
	connect(this, &FunctionPage::windowMoved, this, [combo1, combo2](QPoint globalPos)
		{
			auto movePopups = [globalPos, combo1, combo2](AntComboBox* combo)
				{
					for (PopupViewController* popup : combo->popupViewList())
					{
						// 单层级下拉框
						if (combo == combo1)
						{
							combo->popupViewList()[0]->follow(combo1);
						}
						// 多层下级拉框
						if (combo == combo2)
						{
							combo->popupViewList()[0]->follow(combo2);
							combo2->popupViewList()[1]->follow(combo->popupViewList()[0], PopupViewController::TopRight);
						}
					}
				};
			movePopups(combo1);
			movePopups(combo2);
		});

	// 标签
	QHBoxLayout* row6Layout = new QHBoxLayout();
	row6Layout->setSpacing(10);
	row6Layout->setContentsMargins(0, 22, 0, 22);

	QLabel* tagLabel = new QLabel(QString::fromLocal8Bit("标签"), this);

	// 初始化配置信息列表
	QList<TagWidget::TagInfo> tagList1 = {
		{"bilibili", ":/Imgs/bilibili.svg", QColor(251, 114, 153)},
		{"x",        ":/Imgs/x.svg",        QColor(0, 0, 0)},
		{"wechat",   ":/Imgs/WeChat.svg",   QColor(7, 193, 96)},
		{"taobao",   ":/Imgs/taobao.svg",   QColor(255, 140, 0)},
	};

	QList<TagWidget::TagInfo> tagList2 = {
		{"bilibili", "", QColor(251, 114, 153) },
		{"x",        "", QColor(0, 0, 0)},
		{"wechat",   "", QColor(7, 193, 96)},
		{"taobao",   "", QColor(255, 140, 0)},
	};

	row6Layout->addWidget(tagLabel);

	for (const TagWidget::TagInfo& info : tagList1)
	{
		TagWidget* tag = new TagWidget(info.name, 11.5, info.color, this, true, info.svgPath);
		row6Layout->addWidget(tag);
	}

	for (const TagWidget::TagInfo& info : tagList2)
	{
		TagWidget* tag = new TagWidget(info.name, 11.5, info.color, this, false);
		row6Layout->addWidget(tag);
	}

	// 卡片控件
	QHBoxLayout* row7Layout = new QHBoxLayout();
	row7Layout->setSpacing(10);
	row7Layout->setContentsMargins(0, 22, 0, 22);

	CardWidget* card = new CardWidget("上次登录：xxxx-xx-xx", "总运行时间：xxx小时", this);
	card->setImageFile(":/Imgs/gpt.jpg");
	card->setFixedSize(320, 200);
	QLabel* cardLabel = new QLabel(QString::fromLocal8Bit("卡片"), this);

	// 二维码
	QrCodeWidget* qrCode = new QrCodeWidget(this);
	qrCode->setMinimumSize(QSize(240, 240));
	qrCode->setData(QString("https://ant-design.antgroup.com/index-cn"));
	QLabel* qrCodeLabel = new QLabel(QString::fromLocal8Bit("二维码"), this);

	// 抽屉 注意父对象要设置为主窗口
	QHBoxLayout* row8Layout = new QHBoxLayout();
	row8Layout->setSpacing(10);
	DrawerWidget* drawer = new DrawerWidget(280, DesignSystem::instance()->getMainWindow());
	AntButton* drawerBtn = new AntButton(QString::fromLocal8Bit("打开左侧抽屉"), 12, w1);
	drawerBtn->setFixedSize(140, 50);
	connect(this, &FunctionPage::resized, this, [=](int w, int h)
		{
			drawer->setFixedHeight(h);
		});
	connect(drawerBtn, &AntButton::clicked, this, [=]()
		{
			if (!drawer->isVisible())
			{
				DesignSystem::instance()->getDarkMask()->showAnim();
				drawer->showDrawer();
			}
		});

	// 图标按钮
	AntButton* antIconBtn = new  AntButton("", 12, w1);
	antIconBtn->setSvgIcon(":/Imgs/back.svg");
	antIconBtn->setFixedSize(50, 50);

	// 徽章
	// 创建徽章和标签列表
	QList<BadgeWidget*> badgeWidgets = {
		new BadgeWidget(10, 10, this),  // 圆形徽章
		new BadgeWidget(42, 22, this),  // 圆角矩形徽章
		new BadgeWidget(10, 10, this),  // 成功徽章
		new BadgeWidget(10, 10, this),  // 错误徽章
		new BadgeWidget(10, 10, this),  // 运行中徽章
		new BadgeWidget(10, 10, this),  // 警告徽章
		new BadgeWidget(10, 10, this)   // 默认徽章
	};

	// 设置徽章的状态
	badgeWidgets[0]->setBadge(true, BadgeWidget::None);
	badgeWidgets[1]->setBadge(false, BadgeWidget::None, 100, 99);
	badgeWidgets[2]->setBadge(true, BadgeWidget::Success);
	badgeWidgets[3]->setBadge(true, BadgeWidget::Error);
	badgeWidgets[4]->setBadge(true, BadgeWidget::Running);
	badgeWidgets[5]->setBadge(true, BadgeWidget::Warning);
	badgeWidgets[6]->setBadge(true, BadgeWidget::Default);

	// 创建标签
	QList<QLabel*> badgeLabelList = {
		new QLabel(QString::fromLocal8Bit("圆形徽章"), this),
		new QLabel(QString::fromLocal8Bit("圆角矩形徽章"), this),
		new QLabel(QString::fromLocal8Bit("成功"), this),
		new QLabel(QString::fromLocal8Bit("错误"), this),
		new QLabel(QString::fromLocal8Bit("运行中"), this),
		new QLabel(QString::fromLocal8Bit("警告"), this),
		new QLabel(QString::fromLocal8Bit("默认"), this)
	};

	// 第九行布局
	QHBoxLayout* row9Layout = new QHBoxLayout();
	row9Layout->setContentsMargins(0, 0, 0, 20);
	row9Layout->setSpacing(10);

	// 循环添加标签和徽章
	for (int i = 0; i < badgeLabelList.size(); ++i)
	{
		row9Layout->addWidget(badgeLabelList[i]);
		row9Layout->addWidget(badgeWidgets[i]);
	}

	row9Layout->addStretch();  // 添加伸缩项

	// 标签选项卡
	antTabContainer = new AntTabWidgetContainer(200, 42, true, w1);
	TabContentWidget* testWidget = new TabContentWidget("标签内容Tab 0", w1);
	antTabContainer->addTab("Tab 0", ":/Imgs/git.svg", testWidget);
	QVBoxLayout* row10Layout = new QVBoxLayout();
	QLabel* antTabLabel = new QLabel(QString::fromLocal8Bit("标签选项卡"), this);
	// 外部包裹的容器
	container = new QWidget(w1);
	container->setObjectName("TabContainer");
	container->setStyleSheet(QString("#TabContainer{background-color: %1;}")
		.arg(DesignSystem::instance()->currentTheme().tabContainerColor.name()));
	connect(DesignSystem::instance(), &DesignSystem::themeChanged, this, [this]()
		{
			container->setStyleSheet(QString("#TabContainer{background-color: %1;}")
				.arg(DesignSystem::instance()->currentTheme().tabContainerColor.name()));
		});
	QHBoxLayout* cLayout = new QHBoxLayout(container);
	cLayout->setContentsMargins(20, 20, 20, 20);
	cLayout->addLayout(row10Layout);

	// 轮播图布局
	carouselLayout->addStretch();
	carouselLayout->addWidget(carousel);
	carouselLayout->addStretch();

	// 第一行布局
	row1Layout->addWidget(labelList[0]);
	row1Layout->addWidget(toggleBtn);
	row1Layout->addWidget(labelList[1]);
	row1Layout->addWidget(progress);
	row1Layout->addWidget(spinner);
	row1Layout->addStretch();  // 让它贴左边

	// 第二行布局
	row2Layout->addWidget(labelList[3]);
	row2Layout->addWidget(animNum);
	row2Layout->addWidget(radioBtn1);
	row2Layout->addWidget(radioBtn2);
	row2Layout->addWidget(labelList[2]);
	row2Layout->addWidget(slider);
	row2Layout->addStretch();

	// 第四行布局
	row4Layout->addWidget(taskBtn);
	row4Layout->addWidget(spinBoxLabel1);
	row4Layout->addWidget(spinbox);
	row4Layout->addWidget(spinBoxLabel2);
	row4Layout->addWidget(doubleSpinBox);
	row4Layout->addStretch();

	// 第五行布局
	row5Layout->addWidget(comboLabel1);
	row5Layout->addWidget(combo1);
	row5Layout->addWidget(comboLabel2);
	row5Layout->addWidget(combo2);
	row5Layout->addStretch();

	// 第六行布局
	row6Layout->addStretch();

	// 第七行布局
	row7Layout->addWidget(cardLabel);
	row7Layout->addWidget(card);
	row7Layout->addWidget(qrCodeLabel);
	row7Layout->addWidget(qrCode);
	row7Layout->addStretch();

	row8Layout->addWidget(drawerBtn);
	row8Layout->addWidget(antIconBtn);
	row8Layout->addStretch();

	// 第十行布局
	row10Layout->addWidget(antTabLabel);
	row10Layout->addWidget(antTabContainer);

	// 添加到页面布局
	pageLay->addLayout(carouselLayout);
	pageLay->addLayout(row1Layout);
	pageLay->addLayout(row2Layout);
	pageLay->addWidget(skeletonDescBtn);
	pageLay->addLayout(row3Layout);
	pageLay->addLayout(row4Layout);
	pageLay->addLayout(row5Layout);
	pageLay->addLayout(row6Layout);
	pageLay->addLayout(row7Layout);
	pageLay->addLayout(row8Layout);
	pageLay->addLayout(row9Layout);
	pageLay->addWidget(container);
	pageLay->addStretch();
}

FunctionPage::~FunctionPage()
{
}

void FunctionPage::resizeEvent(QResizeEvent* event)
{
	container->setFixedWidth(width() - 40);
	container->setMinimumHeight(static_cast<int>(width() * 9.0 / 16.0));
}

void FunctionPage::initViewPage()
{
	scrollArea2 = new AntScrollArea(AntScrollArea::ScrollVertical, this);
	QWidget* w2 = new QWidget(this);
	scrollArea2->addWidget(w2);
	// 创建聊天项数据
	QVector<AntChatListView::ChatItem> chatItems = {
	{":/Imgs/bee.png", QString::fromLocal8Bit("张三"), QString::fromLocal8Bit("你好，最近怎么样？"), QString::fromLocal8Bit("10:30 AM"), false},
	{":/Imgs/bee.png", QString::fromLocal8Bit("李四"), "我很好，谢谢！你呢？", "10:31 AM", true},
	{":/Imgs/bee.png", QString::fromLocal8Bit("王五"), "我们今天见面吗？", "10:32 AM", false},
	{":/Imgs/bee.png", QString::fromLocal8Bit("赵六"), "今天晚上有空吗？", "10:33 AM", false},
	{":/Imgs/bee.png", QString::fromLocal8Bit("孙七"), "今晚八点见！", "10:34 AM", true},
	{":/Imgs/bee.png", QString::fromLocal8Bit("周八"), "好的，八点见！", "10:35 AM", false},
	{":/Imgs/bee.png", QString::fromLocal8Bit("吴九"), "你最近在忙什么？", "10:36 AM", false},
	{":/Imgs/bee.png", QString::fromLocal8Bit("郑十"), "最近工作挺忙的，快累死了", "10:37 AM", true},
	{":/Imgs/bee.png", QString::fromLocal8Bit("冯十一"), "加油！工作顺利啊！", "10:38 AM", false},
	{":/Imgs/bee.png", QString::fromLocal8Bit("陈十二"), "谢谢，努力！", "10:39 AM", true},
	{":/Imgs/bee.png", QString::fromLocal8Bit("李十三"), "晚安，明天见！", "10:40 AM", false},
	{":/Imgs/bee.png", QString::fromLocal8Bit("王十四"), "晚安，做个好梦！", "10:41 AM", true},
	{":/Imgs/bee.png", QString::fromLocal8Bit("刘十五"), "明天的计划是什么？", "10:42 AM", false},
	{":/Imgs/bee.png", QString::fromLocal8Bit("张十六"), "明天要去参加会议！", "10:43 AM", true},
	{":/Imgs/bee.png", QString::fromLocal8Bit("黄十七"), "祝你成功！", "10:44 AM", false},
	// 更多数据...
	};
	// 创建你的自定义列表视图
	AntChatListView* chatList = new AntChatListView(w2);
	// 用数据创建模型
	QStandardItemModel* listModel = chatList->createModel(chatItems);
	// 给视图设置模型
	chatList->setModel(listModel);
	chatList->setFixedHeight(600);
	// 视图页布局
	QVBoxLayout* w2Lay = new QVBoxLayout(w2);
	QLabel* listViewLab = new QLabel(QString::fromLocal8Bit("列表视图"), w2);
	listViewLab->setFixedHeight(20);
	w2Lay->setContentsMargins(10, 0, 10, 0);
	w2Lay->setSpacing(10);

	// 表格
	QLabel* tableViewLab = new QLabel(QString::fromLocal8Bit("表格视图"), w2);
	TableColumnLayout colLayout = {
	60,         // avatarWidth
	80,         // actionWidth
	0.25,       // nameRatio
	0.10,       // ageRatio
	0.35,       // addrRatio
	0.30        // tagRatio
	};

	int tagAlpha = 200; // 控制整体标签透明度（0~255）

	rowItems = {
	{
		QPixmap(":/Imgs/bee.png"),
		QString::fromLocal8Bit("张三"),
		"28",
		QString::fromLocal8Bit("北京市朝阳区"),
		{
			{ QString::fromLocal8Bit("程序员"), QColor(255, 87, 34, tagAlpha) },       // 活跃橙（#FF5722）
			{ QString::fromLocal8Bit("北京"), QColor(33, 150, 243, tagAlpha) },        // 明亮蓝（#2196F3）
			{ QString::fromLocal8Bit("技术狂人"), QColor(76, 175, 80, tagAlpha) }       // 草绿色（#4CAF50）
		},
		QString::fromLocal8Bit("编辑")
	},
	{
		QPixmap(":/Imgs/bee.png"),
		QString::fromLocal8Bit("李四"),
		"34",
		QString::fromLocal8Bit("上海市浦东新区"),
		{
			{ QString::fromLocal8Bit("设计师"), QColor(156, 39, 176, tagAlpha) },       // 鲜紫（#9C27B0）
			{ QString::fromLocal8Bit("上海"), QColor(63, 81, 181, tagAlpha) },          // 靛蓝（#3F51B5）
			{ QString::fromLocal8Bit("创意达人"), QColor(233, 30, 99, tagAlpha) }       // 亮粉（#E91E63）
		},
		QString::fromLocal8Bit("删除")
	},
	{
		QPixmap(":/Imgs/bee.png"),
		QString::fromLocal8Bit("王五"),
		"22",
		QString::fromLocal8Bit("广州市天河区"),
		{
			{ QString::fromLocal8Bit("学生"), QColor(96, 125, 139, tagAlpha) },         // 蓝灰（#607D8B）
			{ QString::fromLocal8Bit("广州"), QColor(121, 85, 72, tagAlpha) }           // 深棕（#795548）
		},
		QString::fromLocal8Bit("查看")
	},
	{
		QPixmap(":/Imgs/bee.png"),
		QString::fromLocal8Bit("赵六"),
		"45",
		QString::fromLocal8Bit("深圳市南山区"),
		{
			{ "CEO", QColor(255, 193, 7, tagAlpha) },           // 琥珀黄（#FFC107）
			{ QString::fromLocal8Bit("深圳"), QColor(0, 150, 136, tagAlpha) },          // 青绿（#009688）
			{ QString::fromLocal8Bit("企业家"), QColor(139, 195, 74, tagAlpha) }        // 浅绿（#8BC34A）
		},
		QString::fromLocal8Bit("详情")
	},
	{
		QPixmap(":/Imgs/bee.png"),
		QString::fromLocal8Bit("钱七"),
		"30",
		QString::fromLocal8Bit("武汉市江汉区"),
		{
			{ QString::fromLocal8Bit("项目经理"), QColor(121, 85, 72, tagAlpha) },      // 棕色
			{ QString::fromLocal8Bit("武汉"), QColor(103, 58, 183, tagAlpha) },         // 深紫（#673AB7）
			{ QString::fromLocal8Bit("团队领袖"), QColor(255, 152, 0, tagAlpha) }       // 鲜橙（#FF9800）
		},
		QString::fromLocal8Bit("编辑")
	},
	{
		QPixmap(":/Imgs/bee.png"),
		QString::fromLocal8Bit("孙八"),
		"25",
		QString::fromLocal8Bit("成都市武侯区"),
		{
			{ QString::fromLocal8Bit("产品经理"), QColor(63, 81, 181, tagAlpha) },      // 靛蓝
			{ QString::fromLocal8Bit("成都"), QColor(33, 150, 243, tagAlpha) },         // 蓝
			{ QString::fromLocal8Bit("产品思维"), QColor(76, 175, 80, tagAlpha) }       // 绿
		},
		QString::fromLocal8Bit("删除")
	},
	{
		QPixmap(":/Imgs/bee.png"),
		QString::fromLocal8Bit("周九"),
		"40",
		QString::fromLocal8Bit("重庆市渝中区"),
		{
			{ QString::fromLocal8Bit("设计总监"), QColor(156, 39, 176, tagAlpha) },     // 紫
			{ QString::fromLocal8Bit("重庆"), QColor(233, 30, 99, tagAlpha) }           // 粉
		},
		QString::fromLocal8Bit("查看")
	},
	{
		QPixmap(":/Imgs/bee.png"),
		QString::fromLocal8Bit("吴十"),
		"33",
		QString::fromLocal8Bit("杭州市西湖区"),
		{
			{ QString::fromLocal8Bit("营销总监"), QColor(244, 67, 54, tagAlpha) },      // 红（#F44336）
			{ QString::fromLocal8Bit("杭州"), QColor(0, 150, 136, tagAlpha) },          // 青绿
			{ QString::fromLocal8Bit("互联网营销"), QColor(255, 193, 7, tagAlpha) }     // 琥珀黄
		},
		QString::fromLocal8Bit("详情")
	},
	{
		QPixmap(":/Imgs/bee.png"),
		QString::fromLocal8Bit("郑十一"),
		"27",
		QString::fromLocal8Bit("南京市秦淮区"),
		{
			{ QString::fromLocal8Bit("技术支持"), QColor(33, 150, 243, tagAlpha) },     // 蓝
			{ QString::fromLocal8Bit("南京"), QColor(76, 175, 80, tagAlpha) }           // 绿
		},
		QString::fromLocal8Bit("编辑")
	},
	{
		QPixmap(":/Imgs/bee.png"),
		QString::fromLocal8Bit("冯十二"),
		"38",
		QString::fromLocal8Bit("厦门市思明区"),
		{
			{ QString::fromLocal8Bit("市场经理"), QColor(255, 87, 34, tagAlpha) },      // 橙
			{ QString::fromLocal8Bit("厦门"), QColor(121, 85, 72, tagAlpha) }           // 棕
		},
		QString::fromLocal8Bit("删除")
	}
	};

	table = new AntProfileTable(60, rowItems.size(), 60, colLayout, w2);
	tableModel = table->createModel(rowItems);
	table->setModel(tableModel);
	tableViewLab->setFixedHeight(20);

	// 插入按钮 （根据具体业务来实现）
	// name列和action列插入按钮
	auto theme = DesignSystem::instance()->currentTheme();
	for (int row = 0; row < tableModel->rowCount(); ++row)
	{
		// 获取按钮文本
		QString name = tableModel->item(row, 1)->text();
		QString action = tableModel->item(row, 5)->text();

		AntCellWidget* nameCell = new AntCellWidget(name, table);
		AntCellWidget* actionCell = new AntCellWidget(action, table);

		// 插入按钮到名字列和操作列
		table->setIndexWidget(tableModel->index(row, 1), nameCell);		// 名字列
		table->setIndexWidget(tableModel->index(row, 5), actionCell);		// 操作列
	}

	// 分页器
	PaginationWidget* pagination = new PaginationWidget(QSize(35, 35), w2);
	pagination->setFixedHeight(50);
	pagination->setTotalPages(20);
	pagination->setCurrentPage(1);
	QHBoxLayout* pagiLay = new QHBoxLayout();
	pagiLay->addStretch();
	pagiLay->addWidget(pagination);
	pagiLay->addStretch();

	// 表格+分页器
	QVBoxLayout* vLay = new QVBoxLayout();
	vLay->setContentsMargins(0, 0, 0, 0);
	vLay->setSpacing(0);
	vLay->addWidget(table);
	vLay->addLayout(pagiLay);

	auto loadPage = [this](int page)
		{
			int rowsPerPage = rowItems.size();
			QRegularExpression re("^第\\d+页-");  // 匹配开头“第X页-”

			for (int i = 0; i < rowsPerPage; ++i)
			{
				AntCellWidget* cell = qobject_cast<AntCellWidget*>(table->indexWidget(tableModel->index(i, 1)));

				QString currentText;
				if (cell)
				{
					currentText = cell->getBtn()->text();
				}

				// 用 QRegularExpression 去除前缀
				QRegularExpressionMatch match = re.match(currentText);
				QString baseName = currentText;
				if (match.hasMatch())
				{
					baseName = currentText.mid(match.capturedLength());
				}

				QString newName = QString("第%1页-%2").arg(page).arg(baseName);
				cell->getBtn()->setText(newName);
			}
		};

	loadPage(1);

	connect(pagination, &PaginationWidget::currentPageChanged, this, [=](int page)
		{
			loadPage(page);
		});

	// 树视图
	AntTreeView* tree = new AntTreeView(60, w2);
	QLabel* treeViewLab = new QLabel(QString::fromLocal8Bit("树视图"), w2);

	QList<AntTreeView::ItemData> peoples = {
	{
		QString::fromLocal8Bit("张三"), QIcon(":/Imgs/bee.png"),
		{
			{ "张三的儿子1", QIcon(":/Imgs/bee.png") },
			{ "张三的儿子2", QIcon(":/Imgs/bee.png") }
		}
	},
	{
		QString::fromLocal8Bit("李四"), QIcon(":/Imgs/bee.png"),
		{
			{ "李四的女儿1", QIcon(":/Imgs/bee.png") },
			{ "李四的女儿2", QIcon(":/Imgs/bee.png") }
		}
	},
	{
		QString::fromLocal8Bit("王五"), QIcon(":/Imgs/bee.png"),
		{
			{ "王五的儿子1", QIcon(":/Imgs/bee.png") },
			{
				"王五的儿子2", QIcon(":/Imgs/bee.png"),
				{
					{ "王五的孙子1", QIcon(":/Imgs/bee.png") },
					{ "王五的孙子2", QIcon(":/Imgs/bee.png") }
				}
			},
			{ "王五的儿子3", QIcon(":/Imgs/bee.png") }
		}
	},
	{
		QString::fromLocal8Bit("赵六"), QIcon(":/Imgs/bee.png"),
		{
			{ "赵六的女儿1", QIcon(":/Imgs/bee.png") }
		}
	},
	{
		QString::fromLocal8Bit("钱七"), QIcon(":/Imgs/bee.png"),
		{
			{ "钱七的儿子1", QIcon(":/Imgs/bee.png") },
			{
				"钱七的女儿2", QIcon(":/Imgs/bee.png"),
				{
					{ "钱七的外孙1", QIcon(":/Imgs/bee.png") }
				}
			},
			{ "钱七的儿子3", QIcon(":/Imgs/bee.png") },
			{ "钱七的女儿4", QIcon(":/Imgs/bee.png") }
		}
	},
	{
		QString::fromLocal8Bit("孙八"), QIcon(":/Imgs/bee.png"),
		{
			{ "孙八的儿子1", QIcon(":/Imgs/bee.png") }
		}
	},
	{
		QString::fromLocal8Bit("周九"), QIcon(":/Imgs/bee.png"),
		{
			{ "周九的儿子1", QIcon(":/Imgs/bee.png") },
			{ "周九的儿子2", QIcon(":/Imgs/bee.png") }
		}
	}
	};

	QStandardItemModel* treeModel = tree->createModel(peoples);
	tree->setModel(treeModel);
	tree->calculateFullHeight();

	w2Lay->addWidget(listViewLab);
	w2Lay->addWidget(chatList);
	w2Lay->addWidget(tableViewLab);
	w2Lay->addLayout(vLay);
	w2Lay->addWidget(treeViewLab);
	w2Lay->addWidget(tree);
}