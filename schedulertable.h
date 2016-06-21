#ifndef SCHEDULERTABLE_H
#define SCHEDULERTABLE_H


#include <QTableView>
#include <QMouseEvent>
#include "busmantablemodel.h"


class SchedulerTable: public QTableView
{
    public:
        SchedulerTable();

    public:
        BusmanTableModel model;

};


#endif // SCHEDULERTABLE_H
