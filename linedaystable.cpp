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
