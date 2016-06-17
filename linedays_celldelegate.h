#ifndef LINEDAYS_CELLDELEGATE_H
#define LINEDAYS_CELLDELEGATE_H


#include <QStyledItemDelegate>
#include "linedaystable.h"


class LineDays_CellDelegate : public QStyledItemDelegate
{
    public:
        LineDays_CellDelegate(LineDaysTable* parent=nullptr);

    public:
        LineDaysTable* parentTable;

    protected:
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};



#endif // LINEDAYS_CELLDELEGATE_H
