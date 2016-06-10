#include "linedaystable.h"


LineDaysTable::LineDaysTable() : QTableView()
{
    setModel(&model);
    setItemDelegate(new LineDaysTableNS::CellDelegate());

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setVerticalHeader(new LineDaysTableNS::VerticalHeaderView());
}
