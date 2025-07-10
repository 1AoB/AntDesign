#include "AnimatedNumber.h"
#include <QPainter>
#include <QFontMetrics>

AnimatedNumber::AnimatedNumber(QWidget* parent)
	: QWidget(parent)
{
	m_animation = new QVariantAnimation(this);
	m_animation->setEasingCurve(QEasingCurve::InOutCubic);
	m_animation->setLoopCount(-1);

	connect(m_animation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
		setDisplayedValue(value.toInt());
		});
}

int AnimatedNumber::displayedValue() const
{
	return m_displayedValue;
}

void AnimatedNumber::setDisplayedValue(int val)
{
	if (m_displayedValue != val) {
		m_displayedValue = val;
		update();
	}
}

void AnimatedNumber::animateTo(int target)
{
	int diff = qAbs(target - m_displayedValue);
	int maxDuration = 10000;   // ���10��
	int minDuration = 200;     // ���200ms

	// ������ֵ�����ʱ������Խ��ʱ��Խ��������ӳ��
	int duration = qBound(minDuration, diff * 20, maxDuration);
	// diff*20�Ǹ�ϵ�������Ե��ڶ�������

	m_animation->stop();
	m_animation->setDuration(duration);
	m_animation->setStartValue(m_displayedValue);
	m_animation->setEndValue(target);
	m_animation->start();
}

void AnimatedNumber::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	QFont font = p.font();
	font.setPointSize(m_fontSize);
	font.setBold(true);
	p.setFont(font);
	p.setPen(Qt::black);

	QString text = QString::number(m_displayedValue);
	QFontMetrics fm(font);

	// �����ı��ı߽����
	QRect textRect = fm.boundingRect(text);

	// �������λ��
	QRect drawRect(0, 0, width(), height());
	p.drawText(drawRect, Qt::AlignCenter, text);
}

QSize AnimatedNumber::sizeHint() const
{
	QFont font;
	font.setPointSize(m_fontSize);
	font.setBold(true);
	QFontMetrics fm(font);
	int h = fm.height() + 10; // ��������ռ�
	int w = m_textWidth;              // �̶���ȣ�������һ������ֵ
	return QSize(w, h);
}