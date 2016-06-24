#ifndef VERTICALSHEDULERHEADERVIEW_H
#define VERTICALSHEDULERHEADERVIEW_H


#include <QHeaderView>
#include <QPainter>
#include "scheduler_table_model.h"
#include <QDebug>


class VerticalSchedulerHeaderView: public QHeaderView
{
    Q_OBJECT

    public:
        VerticalSchedulerHeaderView(QWidget* parent = Q_NULLPTR);

    protected:
        void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const;
};

#endif // VERTICALSHEDULERHEADERVIEW_H
