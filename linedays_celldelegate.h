#ifndef LINEDAYS_CELLDELEGATE_H
#define LINEDAYS_CELLDELEGATE_H


#include <QStyledItemDelegate>
#include "linedaystable.h"


class LineDays_CellDelegate : public QStyledItemDelegate
{
    public:
        LineDays_CellDelegate(LineDaysTable* parent=nullptr);
//        LineDays_CellDelegate(QWidget* parent=nullptr): QStyledItemDelegate() {
//            this->parentTable = parent;
//        }

    public:
//            enum SelectedSide {
//                None,
//                Left,
//                Right,
//            };
//            SelectedSide side = SelectedSide::None;

        LineDaysTable* parentTable;

    protected:
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};



#endif // LINEDAYS_CELLDELEGATE_H
