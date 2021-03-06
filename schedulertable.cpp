#include "schedulertable.h"
#include "scheduler_celldelegate.h"
#include "verticalschedulerheaderview.h"


SchedulerTable::SchedulerTable()
    : QTableView()
{
    setModel(&model);
    setItemDelegate(new SchedulerCellDelegate());

    // Заголовок будет в lineDaysTable
    horizontalHeader()->hide();
    setVerticalHeader(new VerticalSchedulerHeaderView());

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

int SchedulerTable::rowCount() const {
    return model.rowCount();
}

int SchedulerTable::columnCount() const {
    return model.columnCount();
}
