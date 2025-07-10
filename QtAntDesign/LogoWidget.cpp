#include "LogoWidget.h"
#include <QPainter>

LogoWidget::LogoWidget(QWidget* parent)
    : QWidget(parent)
{
    // ����ͼƬ��Դ
    m_logoPixmap.load(":/Imgs/logo.png");
}

LogoWidget::~LogoWidget()
{
}

void LogoWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QSize widgetSize = this->size();

    // �������ű��������� 0.7
    double scaleFactor = 0.7;
    QSize scaledSize = widgetSize * scaleFactor;

    QPixmap scaledPixmap = m_logoPixmap.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // �������λ�ã�����ڿؼ���
    int x = (widgetSize.width() - scaledPixmap.width()) / 2;
    int y = (widgetSize.height() - scaledPixmap.height()) / 2;

    painter.drawPixmap(x, y, scaledPixmap);
}
