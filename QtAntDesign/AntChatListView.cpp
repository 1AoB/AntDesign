﻿#include "AntChatListView.h"
#include "StyleSheet.h"
#include "DesignSystem.h"

AntChatListView::AntChatListView(QWidget* parent)
	: AntBaseListView(parent)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QFont font = this->font();
	font.setPointSizeF(11.5);
	setFont(font);

	setStyleSheet(StyleSheet::vListViewQss(DesignSystem::instance()->currentTheme().popupScrollBarColor));

	// 自定义 Item Delegate
	setItemDelegate(new AntChatListItemDelegate(this));
}

AntChatListView::~AntChatListView()
{
}

QStandardItemModel* AntChatListView::createModel(const QVector<ChatItem>& chatItems)
{
	auto model = new QStandardItemModel(this);

	for (const auto& item : chatItems)
	{
		QStandardItem* itemRow = new QStandardItem();

		// 存头像路径
		itemRow->setData(item.avatarPath, Qt::DecorationRole);
		// 存用户名
		itemRow->setData(item.name, Qt::UserRole + 1);
		// 存消息内容
		itemRow->setData(item.message, Qt::UserRole + 2);
		// 存时间
		itemRow->setData(item.time, Qt::UserRole + 3);
		// 是否自己发送
		itemRow->setData(item.isSentByMe, Qt::UserRole + 4);

		model->appendRow(itemRow);
	}

	return model;
}