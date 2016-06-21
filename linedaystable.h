#ifndef LINEDAYSTABLE_H
#define LINEDAYSTABLE_H


#include <QTableView>
#include <QDebug>
#include <QMouseEvent>
#include <QFocusEvent>

#include "linedaystablemodel.h"
#include "linedays_celldelegate.h"


class LineDaysTable : public QTableView
{
    Q_OBJECT

    public:
        LineDaysTable();

        // TODO:
        LineDaysTableModel model;

        // TODO:
        LineDays_CellDelegate delegate;
};


#endif // LINEDAYSTABLE_H
