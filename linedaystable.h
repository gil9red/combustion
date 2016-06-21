#ifndef LINEDAYSTABLE_H
#define LINEDAYSTABLE_H


#include <QTableView>
#include "linedaystablemodel.h"
#include <QDebug>
#include <QMouseEvent>
#include <QFocusEvent>


class LineDaysTable : public QTableView
{
    Q_OBJECT

    public:
        LineDaysTable();

        // TODO:
        LineDaysTableModel model;

        // TODO
        QPoint clickedPos = QPoint(-1, -1);

    protected:
        // TODO: cpp
        void mouseReleaseEvent(QMouseEvent* event) {
            clickedPos = event->pos();

            QModelIndex index = indexAt(clickedPos);
            if (index.isValid()) {
                update(index);
            }

            QTableView::mouseReleaseEvent(event);
        }
};


#endif // LINEDAYSTABLE_H
