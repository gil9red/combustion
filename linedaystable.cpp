#include "linedaystable.h"
#include "linedays_celldelegate.h"
#include "linedays_verticalheaderview.h"


LineDaysTable::LineDaysTable()
    : QTableView()
{
    setModel(&model);

    auto delegate = new LineDays_CellDelegate();
    setItemDelegate(delegate);
    viewport()->installEventFilter(delegate);

    setVerticalHeader(new LineDays_VerticalHeaderView());

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
}
