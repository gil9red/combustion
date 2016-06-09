#include "linedaystable.h"

LineDaysTable::LineDaysTable() : QTableView()
{
    setModel(&model);
    setItemDelegate(new LineDaysTableNS::CellDelegate());

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

//void LineDaysTableNS::CellDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
////    //такой себе костыль, чтобы побороть баг в старых версиях Qt
////    QStyleOptionViewItemV4 viewOption = option;

////    //сохраняем состояние полотна, рисуем фон, восстанавливаем полотно
////    painter->save();

////    QBrush brush = viewOption.backgroundBrush;
////    QColor textColor = Qt::black;
////    QRect rect = viewOption.rect;

////    Busman* busman = index.model()->data(index, BusmanTableModel::BusmanRole).value<Busman*>();
////    QString textCell = index.model()->data(index, BusmanTableModel::WishDayRole).toString();

////    if (textCell == "RR") {
////        brush = QBrush(Qt::lightGray);

////    } else if (textCell == "XX") {
////        textColor = Qt::white;
////        brush = QBrush(Qt::black);

////    } else if (textCell == "DD") {
////        brush = QBrush(Qt::yellow);
////        rect.setWidth(rect.width() / 2);

////    } else if (textCell == "NN") {
////        brush = QBrush(Qt::yellow);
////        rect.setX(rect.x() + rect.width() / 2);
////    }

////    painter->fillRect(rect, brush);

////    painter->restore();

//    //здесь дорисовываются стандартные вещи вроде текста, которые берется из модели
//    QStyledItemDelegate::paint( painter, option, index );
//}
