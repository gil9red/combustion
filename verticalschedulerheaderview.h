#ifndef VERTICALSHEDULERHEADERVIEW_H
#define VERTICALSHEDULERHEADERVIEW_H


#include <QHeaderView>
#include <QPainter>
#include "busmantablemodel.h"
#include <QDebug>


class VerticalSchedulerHeaderView: public QHeaderView
{
    Q_OBJECT

    public:
        VerticalSchedulerHeaderView(QWidget* parent = Q_NULLPTR);
        int scrollX = 0;

    protected:
        // TODO: вынести в cpp
        void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const {
            BusmanTableModel* busmanTableModel = dynamic_cast<BusmanTableModel*> (model());
            if (busmanTableModel != nullptr) {
                Busman* busman = busmanTableModel->get(logicalIndex);
                if (busman != nullptr) {
                    // Draw background
                    painter->save();
                    QStyleOptionHeader option;
                    initStyleOption(&option);
                    option.rect = rect;
                    // TODO: немного имзенить цвет заднего фона -- иначе сложно отличить
                    // от ячеек таблицы
                    style()->drawControl(QStyle::CE_HeaderSection, &option, painter);
//                    painter->setBrush(Qt::black);
//                    painter->drawRect(rect);

                    const int indent = 10;
                    const int vert_indent = 6;
                    int x = rect.x() + indent;
                    int y = rect.y() + vert_indent;
                    int width = 15; // Ширина, которая дается для отрисовки символа в заголовке
                    int height = rect.height() - vert_indent * 2;

                    auto font = painter->font();
                    font.setBold(true);
                    painter->setFont(font);
                    painter->drawText(x, y, width, height, Qt::AlignLeft | Qt::AlignVCenter, busman->busNum);

                    painter->setPen(Qt::black);

                    // Немного отступа после номера автобуса
                    x += 5;
                    for (auto strLine: busman->selectLines) {
                        // Переход для отрисовки следующей цифры и немного отступа для просвета между ними
                        x += width + 2;

                        auto line = busmanTableModel->stringLineMap[strLine];
                        auto color = busmanTableModel->linesColorMap[line];

                        painter->setBrush(color);

                        // Отрисовка рамки и текста в ней
                        painter->drawRect(x, y, width, height);
                        painter->drawText(x, y, width, height, Qt::AlignCenter, strLine);
                    }

                    painter->restore();

                    return;
                }
            }

            QHeaderView::paintSection(painter, rect, logicalIndex);
        }
};

#endif // VERTICALSHEDULERHEADERVIEW_H
