#ifndef LINEDAYSTABLE_H
#define LINEDAYSTABLE_H


#include <QTableView>
#include "linedaystablemodel.h"
#include <QDebug>


class LineDaysTable : public QTableView
{
    Q_OBJECT

    public:
        LineDaysTable();

        // TODO:
        LineDaysTableModel model;

    private slots:
        void clickedIndex(const QModelIndex& index) {
            auto pos = viewport()->mapFromGlobal(QCursor::pos());
            qDebug() << index << pos;
        }
};


#endif // LINEDAYSTABLE_H
