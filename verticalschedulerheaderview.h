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

    protected:
        // TODO: вынести в cpp
        void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const {
            BusmanTableModel* busmanTableModel = dynamic_cast<BusmanTableModel*> (model());
            if (busmanTableModel != nullptr) {
                Busman* busman = busmanTableModel->get(logicalIndex);
                if (busman != nullptr) {
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


                    const int text_indent = 10;
                    const int vert_indent = 6;
                    int x = rect.x() + text_indent;
                    int y = rect.y() + vert_indent;
                    int width = 15; // Ширина, которая дается для отрисовки символа в заголовке
                    int height = rect.height() - vert_indent * 2;

                    painter->save();

                    auto font = painter->font();
                    font.setBold(true);
                    painter->setFont(font);
                    painter->drawText(x, y, width, height, Qt::AlignLeft | Qt::AlignVCenter, busman->busNum);

                    painter->setPen(Qt::black);

                    // Немного отступа после номера автобуса
                    x += 5;

                    for (auto line: busman->lines) {
//                    for (auto strLine: busman->lines) {
                        // Переход для отрисовки следующей цифры и немного отступа для просвета между ними
                        x += width + 2;

//                        auto line = busmanTableModel->stringLineMap[strLine];
                        auto color = busmanTableModel->linesColorMap[line];

                        painter->setBrush(color);

                        // TODO: доработка
                        QString strLine;
                        switch (line) {
                            case Lines::Line_1:
                                strLine = "1";
                                break;

                            case Lines::Line_2:
                                strLine = "2";
                                break;

                            case Lines::Line_3:
                                strLine = "3";
                                break;

                            default:
                                break;
                        }

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
