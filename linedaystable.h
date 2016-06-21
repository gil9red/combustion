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
};


#endif // LINEDAYSTABLE_H
