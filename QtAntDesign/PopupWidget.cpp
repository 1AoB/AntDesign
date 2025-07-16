#include "PopupWidget.h"
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QApplication>
#include <QMouseEvent>
#include "StyleSheet.h"

PopupWidget::PopupWidget(int height, bool enableMultiLevel, QWidget* parent)
	: QWidget(parent),
	m_enableMultiLevel(enableMultiLevel),
	m_popupHeight(height)
{
	// Qt::ToolTip ʼ������� ���ᱻ�ڵ�
	setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	m_listView = new QListView(this);
	m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_listView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_listView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	auto theme = DesignSystem::instance()->currentTheme();
	m_listView->setStyleSheet(StyleSheet::vListViewQss(theme.popupBgColor, theme.popupScrollBarColor));

	// �����Զ��� Item Delegate
	m_listView->setItemDelegate(new ListItemDelegate(36, enableMultiLevel, this));

	// ����
	auto* layout = new QVBoxLayout(this);
	layout->setContentsMargins(10, 10, 10, 10);  // ������Ӱ�ռ�
	layout->addWidget(m_listView);

	// ����
	m_anim = new QPropertyAnimation(this, "popupHeight", this);
	m_anim->setDuration(300);
	m_anim->setEasingCurve(QEasingCurve::OutCubic);

	auto* shadowEffect = new QGraphicsDropShadowEffect(this);
	shadowEffect->setBlurRadius(20);
	shadowEffect->setOffset(0, 0.5);
	shadowEffect->setColor(QColor(50, 50, 50, 120));
	m_listView->setGraphicsEffect(shadowEffect);

	QFont font = m_listView->font();
	font.setPointSize(11);   // ��������
	m_listView->setFont(font);

	connect(m_listView, &QListView::clicked, this, [this, enableMultiLevel](const QModelIndex& idx)
		{
			emit itemSelected(idx);
		});

	connect(m_anim, &QPropertyAnimation::finished, this, [this]()
		{
			if (!m_isVisible)
			{
				hide();
			}
		});
}

void PopupWidget::setModel(QAbstractItemModel* model)
{
	m_listView->setModel(model);
}

void PopupWidget::showAnimated(const QPoint& pos, int width)
{
	if (m_anim->state() == QAbstractAnimation::Running)
		return;  // ��������������У�ֱ�ӷ���

	setFixedWidth(width);
	setPopupHeight(0);							// �߶� 0
	// ����� show() ֮ǰ���� move()��Qt ���¼λ�ã��� OS ���ڹ��������ܺ��Ի򸲸Ǹ�λ�ã��������״���ʾ��
	show();
	move(pos.x(), pos.y() - 8);					// -8 ���أ��Ӿ��ϸ�����

	// ��ȡȫ��λ��
	QPoint mainWindowGlobalPos = DesignSystem::instance()->getMainWindow()->mapToGlobal(QPoint(0, 0));
	QPoint globalPos = QPoint(pos.x(), pos.y() - 8);
	m_offset = globalPos - mainWindowGlobalPos;  // ����ƫ����

	m_isVisible = true;
	m_anim->stop();
	m_anim->setStartValue(0);
	m_anim->setEndValue(m_popupHeight);			// Ŀ��߶�
	m_anim->start();
}

void PopupWidget::hideAnimated()
{
	if (m_anim->state() == QAbstractAnimation::Running)
		return;  // ��������������У�ֱ�ӷ���

	m_isVisible = false;
	m_anim->stop();
	m_anim->setStartValue(height());
	m_anim->setEndValue(0);
	m_anim->start();
}

void PopupWidget::focusOutEvent(QFocusEvent* event)
{
	QWidget::focusOutEvent(event);
	hideAnimated();
}

void PopupWidget::setCurrentIndex(const QModelIndex& index)
{
	if (index.isValid())
	{
		m_listView->setCurrentIndex(index);
	}
}

void PopupWidget::removeAppEventFilter()
{
	qApp->removeEventFilter(this);	// ж��ȫ���¼�������
}