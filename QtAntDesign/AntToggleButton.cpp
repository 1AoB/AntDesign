#include "AntToggleButton.h"
#include <QPainter>
#include <QMouseEvent>

AntToggleButton::AntToggleButton(QSize size, QWidget* parent)
	: QWidget(parent), m_checked(false), m_size(size), m_bgColor("#ccc")
{
	setFixedSize(m_size);

	m_circleWidth = height() - 6;
	m_circleX = 3;

	// ����λ�ö���
	posAnim = new QPropertyAnimation(this, "m_circleX", this);
	posAnim->setDuration(200);
	posAnim->setEasingCurve(QEasingCurve::InOutQuad);

	// ������ɫ����
	colorAnim = new QPropertyAnimation(this, "m_bgColor", this);
	colorAnim->setDuration(200);
	colorAnim->setEasingCurve(QEasingCurve::InOutQuad);

	// ������
	groupAnim = new QParallelAnimationGroup(this);
	groupAnim->addAnimation(posAnim);
	groupAnim->addAnimation(colorAnim);
}

AntToggleButton::~AntToggleButton() {}

void AntToggleButton::setShowText(bool show)
{
	if (m_showText != show)
	{
		m_showText = show;
		update(); // ���»��ƣ���ʾ����������
	}
}

void AntToggleButton::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	// ���Ʊ���
	p.setPen(Qt::NoPen);
	p.setBrush(m_bgColor);
	p.drawRoundedRect(rect(), height() / 2, height() / 2);

	// ���ƻ���
	p.setBrush(Qt::white);
	p.drawEllipse(m_circleX, 3, m_circleWidth, height() - 6);

	// �����Ҫ��ʾ����
	if (m_showText)
	{
		p.setPen(Qt::white);
		QFont font = p.font();
		font.setPointSizeF(9.4);
		font.setBold(true);
		p.setFont(font);

		QString text = m_checked ? QStringLiteral("ON") : QStringLiteral("OFF");
		// ����λ�ã��������ĸ�������΢ƫ�һ�ƫ��㣬����ɵ���
		QFontMetrics fm(font);
		int textWidth = fm.horizontalAdvance(text);
		int textHeight = fm.height();

		int xText;
		if (m_checked)
		{
			// �������ұߣ����ֻ��ڻ������һЩ
			xText = m_circleX - textWidth - 2;
		}
		else
		{
			// ��������ߣ����ֻ��ڻ����ұ�һ��
			xText = m_circleX + m_circleWidth + 1;
		}
		int yText = (height() + textHeight) / 2 - fm.descent();

		p.drawText(xText, yText, text);
	}
}

void AntToggleButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && groupAnim->state() != QAbstractAnimation::Running)
	{
		setChecked(!m_checked);
	}

	QWidget::mousePressEvent(event);
}

void AntToggleButton::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	m_circleWidth = height() - 6;
	m_circleX = m_checked ? width() - height() + 3 : 3;
}

void AntToggleButton::setChecked(bool checked)
{
	if (m_checked != checked) {
		m_checked = checked;

		posAnim->stop();
		posAnim->setStartValue(m_circleX);
		posAnim->setEndValue(m_checked ? width() - height() + 3 : 3);

		colorAnim->stop();
		colorAnim->setStartValue(m_bgColor);
		colorAnim->setEndValue(m_checked ? QColor("#1890ff") : QColor("#ccc"));

		groupAnim->start();

		emit toggled(m_checked);
	}
}