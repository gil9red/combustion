#ifndef CELLDELEGATE_H
#define CELLDELEGATE_H


#include <QStyledItemDelegate>


class CellDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit CellDelegate(QObject *parent = 0);

    protected:
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // CELLDELEGATE_H
