#ifndef LINEDAYS_CELLDELEGATE_H
#define LINEDAYS_CELLDELEGATE_H


#include <QStyledItemDelegate>
#include <QEvent>

#include "enums.h"


class LineDays_CellDelegate : public QStyledItemDelegate {

    public:
        bool eventFilter(QObject* obj, QEvent* event);

    private:
        QPersistentModelIndex mIndex;

    public:
        // TODO:
        Side selectedSide = Side::None;

    protected:
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // LINEDAYS_CELLDELEGATE_H
