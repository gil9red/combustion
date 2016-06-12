#include "linedaystable.h"
#include "linedays_celldelegate.h"
#include "linedays_verticalheaderview.h"


LineDaysTable::LineDaysTable()
    : QTableView()
{
    setModel(&model);
    setItemDelegate(new LineDays_CellDelegate(this->viewport()));
    setVerticalHeader(new LineDays_VerticalHeaderView());

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
}
