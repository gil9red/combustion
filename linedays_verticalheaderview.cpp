#include "linedays_verticalheaderview.h"
#include "linedaystablemodel.h"
#include <QPainter>


LineDays_VerticalHeaderView::LineDays_VerticalHeaderView()
    : QHeaderView(Qt::Vertical) {

}


void LineDays_VerticalHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const {
//                BusmanTableModel* busmanTableModel = dynamic_cast<BusmanTableModel*> (model());
//                if (busmanTableModel != nullptr) {
//                    Busman* busman = busmanTableModel->get(logicalIndex);
//                    if (busman != nullptr) {
//                        QStyleOptionHeader option;
//                        initStyleOption(&option);
//                        option.rect = rect;

//                        // Отрисовка ячейки и затемнение фона, чтобы было отличие от ячеек таблицы
//                        style()->drawControl(QStyle::CE_HeaderSection, &option, painter);
//                        painter->save();
//                        painter->setPen(Qt::NoPen);
//                        painter->setBrush(QColor(0, 0, 0, 20));
//                        painter->drawRect(option.rect);
//                        painter->restore();


//                        const int text_indent = 10;
//                        const int vert_indent = 6;
//                        int x = rect.x() + text_indent;
//                        int y = rect.y() + vert_indent;
//                        int width = 15; // Ширина, которая дается для отрисовки символа в заголовке
//                        int height = rect.height() - vert_indent * 2;

//                        painter->save();

//                        auto font = painter->font();
//                        font.setBold(true);
//                        painter->setFont(font);
//                        painter->drawText(x, y, width, height, Qt::AlignLeft | Qt::AlignVCenter, busman->busNum);

//                        painter->setPen(Qt::black);

//                        // Немного отступа после номера автобуса
//                        x += 5;
//                        for (auto strLine: busman->selectLines) {
//                            // Переход для отрисовки следующей цифры и немного отступа для просвета между ними
//                            x += width + 2;

//                            auto line = busmanTableModel->stringLineMap[strLine];
//                            auto color = busmanTableModel->linesColorMap[line];

//                            painter->setBrush(color);

//                            // Отрисовка рамки и текста в ней
//                            painter->drawRect(x, y, width, height);
//                            painter->drawText(x, y, width, height, Qt::AlignCenter, strLine);
//                        }

//                        painter->restore();

//                        return;
//                    }
//                }

    // TODO: добавить проверку как выше
    LineDaysTableModel* lineDaysTableModel = dynamic_cast<LineDaysTableModel*> (model());
    auto busmanTableModel = lineDaysTableModel->busmanTableModel;

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

    // TODO: неочень хорошо выглядит это получение цвета линии
    auto line = (Lines) (logicalIndex + 1);
    auto color = busmanTableModel->linesColorMap[line];
    painter->setBrush(color);

    // Отрисовка рамки и текста в ней
    painter->drawRect(x, y, width, height);
    painter->drawText(x, y, width, height, Qt::AlignCenter, QString("Линия %1").arg(logicalIndex + 1));

//QHeaderView::paintSection(painter, rect, logicalIndex);
}
