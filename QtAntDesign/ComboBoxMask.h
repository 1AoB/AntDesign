#pragma once

#include <QWidget>

class ComboBoxMask : public QWidget
{
	Q_OBJECT
public:
	explicit ComboBoxMask(QWidget* parent);

signals:
	void clickedOutside(); // ����������򷢳��ź�

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event);
};
