#include "celldelegate.h"

#include <QPainter>
#include <QDebug>

#include "busman.h"
#include "busmantablemodel.h"


void CellDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 viewOption = option;

    painter->save();

    QBrush brush = viewOption.backgroundBrush;
    QColor textColor = Qt::black;
    QRect rect = viewOption.rect;

    // TODO:
//    QString textCell = index.model()->data(index, Qt::DisplayRole).toString();
//    Busman* busman = index.model()->data(index, BusmanTableModel::BusmanRole).value<Busman*>();
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

    painter->restore();

    // Здесь дорисовываются стандартные вещи вроде текста, которые берутся из модели
    QStyledItemDelegate::paint( painter, option, index );
}
