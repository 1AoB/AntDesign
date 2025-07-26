﻿#include "AntInput.h"
#include <QStyle>
#include <QResizeEvent>
#include <QStandardItemModel>

AntInput::AntInput(int popupHeight, QStringList itemTextList, QWidget* parent)
	: AntBaseInput(parent)
{
	m_searchButton = new QToolButton(this);
	m_searchButton->setCursor(Qt::PointingHandCursor);
	m_searchButton->setIcon(QIcon(":/Imgs/search.svg")); // 替换成你的放大镜图标路径
	m_searchButton->setFixedSize(17, 17);
	m_searchButton->setIconSize(QSize(17, 17)); // 设置图标大小
	m_searchButton->setStyleSheet(R"(
    QToolButton {
        border: none;
        padding: 0;
        background: transparent;
    })");

	connect(m_searchButton, &QToolButton::clicked, this, &AntInput::onSearchClicked);

	QFont font = this->font();
	font.setPointSizeF(10.8);  // 自定义大小
	setFont(font);

	popupView = new PopupViewController(popupHeight, false, this);

	connect(popupView, &PopupViewController::itemSelected, this, [this](const QModelIndex& idx)
		{
			// 具体业务逻辑
			setCurrentText(idx.data().toString());
			popupView->hideAnimated();
			clearFocus();
		});

	QStandardItemModel* model1 = new QStandardItemModel(this);
	for (const QString& text : itemTextList)
	{
		QStandardItem* item = new QStandardItem(text);

		// 这里添加自定义控件 item->setData

		model1->appendRow(item);
	}
	popupView->popup->setModel(model1);
}

void AntInput::resizeEvent(QResizeEvent* event)
{
	AntBaseInput::resizeEvent(event);
	updateSearchButtonPosition();
}

void AntInput::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		AntBaseInput::mousePressEvent(event);
		QPoint popupPos = mapToGlobal(QPoint(0, height()));
		popupView->raise();
		popupView->showAnimated(popupPos, width());
	}
}

void AntInput::updateSearchButtonPosition()
{
	int frameWidth = 3;
	int btnSize = m_searchButton->size().width();
	int padding = 12; // 你定义的左侧间距

	// 把按钮放到左侧，距左边框 padding + frameWidth
	m_searchButton->move(frameWidth + padding, (height() - btnSize) / 2);

	// 设置文本左边距，避免文字覆盖按钮
	setTextMargins(btnSize + frameWidth + padding, 0, 0, 0);
}

void AntInput::setCurrentText(QString text)
{
	setText(text);
}

void AntInput::onSearchClicked()
{
}