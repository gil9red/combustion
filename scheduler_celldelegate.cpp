#include "scheduler_celldelegate.h"

#include <QPainter>
#include <QDebug>

#include "busman.h"
#include "scheduler_table_model.h"


void SchedulerCellDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItemV4 viewOption = option;

    painter->save();

    QBrush brush = viewOption.backgroundBrush;
    QColor textColor = Qt::black;
    QRect rect = viewOption.rect;

    QString textCell = index.model()->data(index, SchedulerTableModel::WishDayRole).toString();

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

    DayKind day = index.model()->data(index, SchedulerTableModel::DayKindRole).value<DayKind>();
    if (day != DayKind::NONE) {
        QImage dayImage = index.model()->data(index, SchedulerTableModel::DayImageKindRole).value<QImage>();
        QRect rect = option.rect;

        // Отступ иконки в ячейке
        auto indent = 4;
        auto size = qMin(rect.size().width(), rect.size().height()) - indent * 2;

        if (!dayImage.isNull()) {
            dayImage = dayImage.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }

        if (isDay(day)) {
            painter->drawImage(rect.x() + indent, rect.y() + indent, dayImage);

        } else if (isNight(day)) {
            painter->drawImage(rect.x() + size + indent + indent * 2, rect.y() + indent, dayImage);
        }
    }

    // Если ячейка входит в LongRests ячейки, подрисуем снизу линию, подчеркивая ячейку
    const auto schedulerTableModel = dynamic_cast <const SchedulerTableModel*> (index.model());
    if (schedulerTableModel && schedulerTableModel->columnInLongRests(index)) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(255, 128, 0));

        const auto heightLine = 4;
        auto rect = viewOption.rect;
        rect.setY(rect.y() + rect.height() - heightLine);
        rect.setHeight(heightLine);
        painter->drawRect(rect);
    }

    painter->restore();

    // Цвет выделения полупрозрачный, чтобы были видно что в ячейке
    QStyleOptionViewItem itemOption(option);
    initStyleOption(&itemOption, index);

    if (itemOption.state & QStyle::State_Selected) {
        auto color = itemOption.palette.color(QPalette::Highlight);
        color.setAlpha(180);
        itemOption.palette.setColor(QPalette::Highlight, color);
    }

    // Здесь дорисовываются стандартные вещи вроде текста, которые берутся из модели
    QStyledItemDelegate::paint(painter, itemOption, index);
}
