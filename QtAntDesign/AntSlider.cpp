// AntSlider.cpp
#include "AntSlider.h"
#include <QPainter>
#include <QStyleOptionSlider>
#include <QMouseEvent>
#include <QPainterPath>
#include <QTimer>

AntSlider::AntSlider(int min, int max, int value, QWidget* parent)
	: QSlider(Qt::Horizontal, parent)
{
	// ��ʼ����Χ��ֵ
	setRange(min, max);
	setValue(value);
	setMouseTracking(true);
	setContentsMargins(m_margin, 0, m_margin, 0);

	// ��ʼ�� handle ����
	m_radiusAnimation = new QPropertyAnimation(this, "handleRadius", this);
	m_radiusAnimation->setDuration(180);
	m_radiusAnimation->setEasingCurve(QEasingCurve::InOutQuad);

	// ��ʼ�� tooltip
	m_tooltip = new DownArrowTooltip(nullptr);
	m_tooltip->setText(QString::number(value));
	m_tooltip->hide();

	// �ۺ�������
	connect(this, &QSlider::sliderPressed, this, [this]() {
		m_tooltip->setText(QString::number(this->value()));
		});

	connect(this, &QSlider::valueChanged, this, [this](int val) {
		m_tooltip->setText(QString::number(val));
		});

	connect(this, &AntSlider::handleMoved, this, [=](const QPoint& pt) {
		QPoint global = mapToGlobal(pt);
		m_tooltip->move(global.x() - m_tooltip->width() / 2,
			global.y() - m_tooltip->height() - 15);
		});
}

void AntSlider::setHandleRadius(qreal radius)
{
	if (!qFuzzyCompare(m_handleRadius, radius)) {
		m_handleRadius = radius;
		update();
	}
}

void AntSlider::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	// ---- ��� ---------------------------------------------------
	const QRect tr = trackRect();
	const int   handleX = tr.left()
		+ qRound(double(value() - minimum())
			/ (maximum() - minimum())
			* tr.width());

	p.setPen(Qt::NoPen);
	p.setBrush(QColor(200, 200, 200));                // ����
	p.drawRoundedRect(tr, tr.height() / 2, tr.height() / 2);

	QRect fillRect(tr.left(), tr.top(),
		handleX - tr.left(), tr.height());
	p.setBrush(QColor(66, 133, 244));                 // ����ɲ���
	p.drawRoundedRect(fillRect, tr.height() / 2, tr.height() / 2);

	// ---- ���� ---------------------------------------------------
	const QPointF c(handleX, height() / 2.0);
	const qreal outerR = m_handleRadius;
	const qreal ringW = static_cast<qreal>(m_ringWidth);
	const qreal innerR = outerR - ringW / 2.0;

	QColor ringColor = m_hovered ? QColor(22, 119, 255)
		: QColor(153, 204, 255);

	p.setBrush(Qt::NoBrush);
	p.setPen(QPen(ringColor, ringW));
	p.drawEllipse(c, outerR, outerR);               // �⻷

	p.setPen(Qt::NoPen);
	p.setBrush(Qt::white);
	p.drawEllipse(c, innerR, innerR);               // ����

	// ���们����������
	emit handleMoved(QPoint(handleX, height() / 2));
}

void AntSlider::enterEvent(QEnterEvent* event)
{
	QSlider::enterEvent(event);
	m_hovered = true;
	m_radiusAnimation->stop();
	m_radiusAnimation->setStartValue(m_handleRadius);
	m_radiusAnimation->setEndValue(9);
	m_radiusAnimation->start();
	m_tooltip->fadeIn();
	update();
}

void AntSlider::leaveEvent(QEvent* event)
{
	QSlider::leaveEvent(event);
	m_hovered = false;
	m_radiusAnimation->stop();
	m_radiusAnimation->setStartValue(m_handleRadius);
	m_radiusAnimation->setEndValue(7);
	m_radiusAnimation->start();
	m_tooltip->fadeOut();
	update();
}

void AntSlider::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		const QRect tr = trackRect();
		int pos = std::clamp(e->pos().x(), tr.left(), tr.right());
		// ���ڽ����λ�� pos ӳ��Ϊ��Ӧ�Ļ���ֵ
		int v = QStyle::sliderValueFromPosition(minimum(), maximum(),
			pos - tr.left(),
			tr.width());
		setValue(v);
		e->accept();
	}
	QSlider::mousePressEvent(e);   // ����֧����ק
}

QSize AntSlider::sizeHint() const
{
	QSize s = QSlider::sizeHint();
	s.setHeight(m_maxRadius * 2 + m_ringWidth + 4);
	return s;
}

DownArrowTooltip::DownArrowTooltip(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);

	m_font.setPointSize(12);
	setFont(m_font);

	// ---------- ���͸����Ч�� ----------
	auto* opacityEffect = new QGraphicsOpacityEffect(this);
	setGraphicsEffect(opacityEffect);

	m_opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity", this);
	m_opacityAnimation->setDuration(300);
	m_opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);

	connect(m_opacityAnimation, &QPropertyAnimation::finished, this, [this]() {
		if (isHide)
			this->hide();
		});
}

void DownArrowTooltip::setText(const QString& text)
{
	m_text = text;

	QFontMetrics fm(m_font);
	int textWidth = fm.horizontalAdvance(m_text);
	int totalWidth = textWidth + padding * 2;

	resize(totalWidth, fixedHeight + arrowHeight);
	update();
}

void DownArrowTooltip::showAt(const QPoint& globalPos)
{
	move(globalPos);
	show();
}

// DownArrowTooltip

void DownArrowTooltip::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	const int radius = 6;

	QRect bubbleRect(0, 0, width(), height() - arrowHeight);
	QPainterPath path;

	// Բ�Ǿ��β���
	path.moveTo(radius, 0);
	path.lineTo(width() - radius, 0);
	path.quadTo(width(), 0, width(), radius);
	path.lineTo(width(), bubbleRect.bottom() - radius);
	path.quadTo(width(), bubbleRect.bottom(), width() - radius, bubbleRect.bottom());
	path.lineTo(radius, bubbleRect.bottom());
	path.quadTo(0, bubbleRect.bottom(), 0, bubbleRect.bottom() - radius);
	path.lineTo(0, radius);
	path.quadTo(0, 0, radius, 0);

	// ���¼�ͷ
	int ax = width() / 2;
	int ay = bubbleRect.bottom();
	path.moveTo(ax - arrowWidth / 2, ay);
	path.lineTo(ax, ay + arrowHeight);
	path.lineTo(ax + arrowWidth / 2, ay);
	path.closeSubpath();

	// ��䱳������ɫ
	p.setBrush(QColor(30, 30, 30));  // ��ҽӽ���
	p.setPen(Qt::NoPen);
	p.drawPath(path);

	// ���֣���ɫ
	p.setFont(m_font);
	p.setPen(Qt::white);
	p.drawText(bubbleRect, Qt::AlignCenter, m_text);
}

void DownArrowTooltip::fadeIn()
{
	isHide = false;
	show();
	m_opacityAnimation->stop();
	m_opacityAnimation->setStartValue(0.0);
	m_opacityAnimation->setEndValue(1.0);
	m_opacityAnimation->start();
}

void DownArrowTooltip::fadeOut()
{
	isHide = true;
	m_opacityAnimation->stop();
	m_opacityAnimation->setStartValue(1.0);
	m_opacityAnimation->setEndValue(0.0);
	m_opacityAnimation->start();
}