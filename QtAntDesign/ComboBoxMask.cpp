#include "ComboBoxMask.h"
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QPainter>

ComboBoxMask::ComboBoxMask(QWidget* parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setObjectName("ComboBoxMask");

	// ����Ϊȫ������
	if (parent)
	{
		resize(parent->width(), parent->height());
	}
	hide();
}

void ComboBoxMask::mousePressEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	emit clickedOutside();  // ֪ͨ�ⲿ�رյ����Ȳ���
	event->accept();		// ��ֹ�¼���������
}

void ComboBoxMask::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	// �����ø���������ɫ ����鿴�����Ƿ񸲸�
	//QPainter painter(this);
	//painter.setRenderHint(QPainter::Antialiasing, true);

	//// ����һ����͸����ɫ����
	//QColor maskColor(0, 0, 200, 80);
	//painter.fillRect(rect(), maskColor);
}