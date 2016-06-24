#ifndef SCHEDULERTABLE_H
#define SCHEDULERTABLE_H


#include <QTableView>
#include <QMouseEvent>
#include "scheduler_table_model.h"


class SchedulerTable: public QTableView
{
    public:
        SchedulerTable();

    public:
        SchedulerTableModel model;

};


#endif // SCHEDULERTABLE_H
