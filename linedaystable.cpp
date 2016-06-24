#include "linedaystable.h"
#include "linedays_verticalheaderview.h"


LineDaysTable::LineDaysTable()
    : QTableView()
{
    setModel(&model);

    setItemDelegate(&delegate);
    viewport()->installEventFilter(&delegate);

    setVerticalHeader(new LineDays_VerticalHeaderView());

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

int LineDaysTable::rowCount() const {
    return model.rowCount();
}

int LineDaysTable::columnCount() const {
    return model.columnCount();
}
