#include "linedays_verticalheaderview.h"
#include "linedaystablemodel.h"
#include <QPainter>


LineDays_VerticalHeaderView::LineDays_VerticalHeaderView()
    : QHeaderView(Qt::Vertical) {

}


void LineDays_VerticalHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const {
    LineDaysTableModel* lineDaysTableModel = dynamic_cast<LineDaysTableModel*> (model());
    if (lineDaysTableModel == nullptr) {
        return;
    }

    auto schedulerTableModel = lineDaysTableModel->schedulerTableModel;

    QStyleOptionHeader option;
    initStyleOption(&option);
    option.rect = rect;

    // Отрисовка ячейки и затемнение фона, чтобы было отличие от ячеек таблицы
    style()->drawControl(QStyle::CE_HeaderSection, &option, painter);
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 20));
    painter->drawRect(option.rect);
    painter->restore();

    auto font = painter->font();
    font.setBold(true);
    painter->setFont(font);
    painter->setPen(Qt::black);

    const int text_indent = 10;
    const int vert_indent = 6;
    int x = rect.x() + text_indent;
    int y = rect.y() + vert_indent;
    int width = rect.width() - text_indent * 2;
    int height = rect.height() - vert_indent * 2;

    auto line = (Lines) logicalIndex;
    auto color = schedulerTableModel->linesColorMap[line];
    painter->setBrush(color);

    // Отрисовка рамки и текста в ней
    painter->drawRect(x, y, width, height);
    painter->drawText(x, y, width, height, Qt::AlignCenter, QString("Линия %1").arg(logicalIndex + 1));
}
