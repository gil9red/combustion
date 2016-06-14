#include "scheduler_celldelegate.h"

#include <QPainter>
#include <QDebug>

#include "busman.h"
#include "busmantablemodel.h"


void SchedulerCellDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 viewOption = option;

    painter->save();

    QBrush brush = viewOption.backgroundBrush;
    QColor textColor = Qt::black;
    QRect rect = viewOption.rect;

    // TODO:
//    QString textCell = index.model()->data(index, Qt::DisplayRole).toString();
    QString textCell = index.model()->data(index, BusmanTableModel::WishDayRole).toString();

    if (textCell == "RR") {
        brush = QBrush(Qt::lightGray);

    } else if (textCell == "XX") {
        textColor = Qt::white;
        brush = QBrush(Qt::black);

// TODO: градиент как скрине
//        QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
//        gradient.setSpread(QGradient::RepeatSpread);
//        gradient.setColorAt(0, Qt::black);
//        gradient.setColorAt(1, Qt::white);
//        brush = QBrush(gradient);

    } else if (textCell == "DD") {
        brush = QBrush(Qt::yellow);
        rect.setWidth(rect.width() / 2);

    } else if (textCell == "NN") {
        brush = QBrush(Qt::yellow);
        rect.setX(rect.x() + rect.width() / 2);
    }

    painter->fillRect(rect, brush);

    // TODO: выделение полупрозрачным сделать
    Busman::DayKind day = index.model()->data(index, BusmanTableModel::DayKindRole).value<Busman::DayKind>();
    if (day != Busman::DayKind::NONE) {
        // TODO: Для sun рисовать иконку слева, для moon -- справа
        QImage dayImage = index.model()->data(index, BusmanTableModel::DayImageKindRole).value<QImage>();
        qDebug() << dayImage.isNull() << index << day;

//        painter->drawImage(viewOption.rect.topLeft(), dayImage);

        QRect rect = option.rect;

        // TODO: может busman тоже нужно хранить по две, хоть за день у него не будет две смены?
//        QImage dayImage = index.model()->data(index, LineDaysTableModel::DayKind_Day_Role).value<QImage>();
//        QImage nightImage = index.model()->data(index, LineDaysTableModel::DayKind_Night_Role).value<QImage>();

        // Отступ между иконками в ячейке
        auto indent = 2;
        auto size = qMin(rect.size().width(), rect.size().height()) - indent / 2;

        // TODO: исправить ошибку "QImage::scaled: Image is a null image"
        dayImage = dayImage.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        nightImage = nightImage.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        painter->drawImage(rect.x(), rect.y(), dayImage);
        painter->drawImage(rect.x() + size + indent, rect.y(), dayImage);
    }
    painter->drawText(option.rect, Qt::AlignLeft | Qt::AlignTop, QString::number(day));

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

    // Здесь дорисовываются стандартные вещи вроде текста, которые берутся из модели
    QStyledItemDelegate::paint(painter, itemOption, index);
}
