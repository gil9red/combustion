#ifndef CELLDELEGATE_H
#define CELLDELEGATE_H


#include <QStyledItemDelegate>


class SchedulerCellDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    protected:
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // CELLDELEGATE_H
