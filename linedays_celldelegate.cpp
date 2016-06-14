#include "linedays_celldelegate.h"
#include "linedaystablemodel.h"
#include <QPainter>
#include "linedaystable.h"
#include <QDebug>
#include <QDate>


LineDays_CellDelegate::LineDays_CellDelegate(LineDaysTable* parent)
    : QStyledItemDelegate() {
    this->parentTable = parent;
}

void LineDays_CellDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    QRect rect = option.rect;

    QImage dayImage = index.model()->data(index, LineDaysTableModel::DayKind_Day_Role).value<QImage>();
    QImage nightImage = index.model()->data(index, LineDaysTableModel::DayKind_Night_Role).value<QImage>();

    // Отступ между иконками в ячейке
    auto indent = 2;
    auto size = qMin(rect.size().width(), rect.size().height()) - indent / 2;
    dayImage = dayImage.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    nightImage = nightImage.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter->drawImage(rect.x(), rect.y(), dayImage);
    painter->drawImage(rect.x() + size + indent, rect.y(), nightImage);

    painter->restore();

    // Цвет выделения полупрозрачный, чтобы были видно что в ячейке
    QStyleOptionViewItem itemOption(option);
    initStyleOption(&itemOption, index);

    if ((itemOption.state & QStyle::State_Selected) /*&& (itemOption.state & QStyle::State_Active)*/) {
//        auto pos = parentTable->clickedPos;
//        if (pos != QPoint(-1, -1)) {
//            bool leftSide = (rect.x() + (rect.width() / 2)) > pos.x();
//            qDebug() << QDateTime::currentDateTime() << pos << (leftSide ? "left" : "right");

//            if (leftSide) {
//                itemOption.rect.setWidth(itemOption.rect.width() / 2);
//            } else {
//                itemOption.rect.setX(itemOption.rect.x() + itemOption.rect.width() / 2);
//            }
//        }

        auto color = itemOption.palette.color(QPalette::Highlight);
        color.setAlpha(180);
        itemOption.palette.setColor(QPalette::Highlight, color);
    }

    QStyledItemDelegate::paint(painter, itemOption, index);
}
