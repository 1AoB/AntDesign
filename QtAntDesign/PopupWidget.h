#pragma once

#include <QWidget>
#include <QListView>
#include <QPropertyAnimation>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include "DesignSystem.h"

// �Զ��� Item Delegate ������ ListItem �Ĵ�С
class ListItemDelegate : public QStyledItemDelegate
{
public:
	explicit ListItemDelegate(int itemHieght = 36, bool enableMultiLevel = false, QObject* parent = nullptr)
		: QStyledItemDelegate(parent), m_itemHeight(itemHieght), m_enableMultiLevel(enableMultiLevel)
	{
		// ѡ�еı�����ɫ
		QColor color = DesignSystem::instance()->primaryColor();
		color.setAlpha(30);
		m_curIdxBgColor = color;
		// ��ȡĬ�ϵı���ɫ
		m_bgColor = DesignSystem::instance()->currentTheme().popupItemBgColor;
		// ��ȡĬ�ϵ��ı���ɫ
		m_textColor = DesignSystem::instance()->currentTheme().popupTextColor;
	}

	// ��д sizeHint ��ָ��ÿ�� item �Ĵ�С
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override
	{
		// ��ȡԭʼ�Ľ����С
		QSize originalSize = QStyledItemDelegate::sizeHint(option, index);
		// ����һ���̶��ġ�����ĸ߶ȣ����� 36px������Ը�����Ҫ������
		return QSize(originalSize.width(), m_itemHeight);
	}

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
	{
		painter->save();

		QRectF rect = option.rect.adjusted(2, 2, -2, -2);
		int borderRadius = 6;

		const QListView* listView = qobject_cast<const QListView*>(option.widget);
		QModelIndex currentIndex = listView ? listView->currentIndex() : QModelIndex();

		bool isHovered = option.state & QStyle::State_MouseOver;
		bool isCurrent = index == currentIndex;

		// ������
		if (isCurrent) 
		{
			painter->setBrush(m_curIdxBgColor);
			painter->setPen(Qt::NoPen);
			painter->drawRoundedRect(rect, borderRadius, borderRadius);
		}
		else if (isHovered) 
		{
			painter->setBrush(m_bgColor);
			painter->setPen(Qt::NoPen);
			painter->drawRoundedRect(rect, borderRadius, borderRadius);
		}

		// ��������
		QFont font = option.font;
		if (isCurrent) {
			font.setBold(true);
		}
		painter->setFont(font);

		// ������ɫ
		painter->setPen(m_textColor);

		// �ı�����Ԥ�����Ҳ�ͼ��ռ�
		QRect textRect = option.rect.adjusted(10, 0, -10, 0);
		// ���ı�
		QString text = index.data(Qt::DisplayRole).toString();
		int flags = m_enableMultiLevel ? (Qt::AlignVCenter | Qt::AlignLeft) : Qt::AlignCenter;
		painter->drawText(textRect, flags, text);

		// ��ͼ�꣨�Ҳࣩ
		if (m_enableMultiLevel)
		{
			QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
			if (!icon.isNull()) {
				int iconSize = 12;
				QRect iconRect(option.rect.right() - iconSize - 5,
					option.rect.top() + (option.rect.height() - iconSize) / 2,
					iconSize, iconSize);
				icon.paint(painter, iconRect);
			}
		}

		painter->restore();
	}

private:
	int m_itemHeight;			// ���ڴ洢ÿ�� item �ĸ߶�
	QColor m_bgColor;			// Ĭ�ϵı���ɫ
	QColor m_curIdxBgColor;		// ��ǰѡ����ı�����ɫ
	QColor m_textColor;			// �ı���ɫ
	bool m_enableMultiLevel;	// �Ƿ����ö༶�б�
};

class PopupWidget : public QWidget
{
	Q_OBJECT
		Q_PROPERTY(int popupHeight READ popupHeight WRITE setPopupHeight)
public:
	explicit PopupWidget(int height, bool enableMultiLevel, QWidget* parent = nullptr);
	void setModel(QAbstractItemModel* model);
	void showAnimated(const QPoint& pos, int width);
	void hideAnimated();
	/* ���Դ�ȡ�� */
	int  popupHeight() const { return height(); }
	void setPopupHeight(int h) { setFixedHeight(h); }
	// �����������������õ�ǰ��
	void setCurrentIndex(const QModelIndex& index);
	bool getVisible() { return m_isVisible; }
	void removeAppEventFilter();
signals:
	void itemSelected(const QModelIndex& index);
	void containsMousePos(bool isContains);
protected:
	void focusOutEvent(QFocusEvent* event) override;
public:
	QPoint m_offset;
private:
	QListView* m_listView;
	QPropertyAnimation* m_anim;
	bool m_isVisible;
	bool m_enableMultiLevel;	// �Ƿ����ö༶�б�
	int m_popupHeight;			// ������ĸ߶�
};
