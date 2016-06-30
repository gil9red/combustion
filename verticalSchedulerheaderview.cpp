#include "verticalschedulerheaderview.h"


VerticalSchedulerHeaderView::VerticalSchedulerHeaderView(QWidget* parent)
    : QHeaderView(Qt::Vertical, parent)
{

}

void VerticalSchedulerHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const {
    SchedulerTableModel* schedulerTableModel = dynamic_cast<SchedulerTableModel*> (model());
    if (schedulerTableModel) {
        Busman* busman = schedulerTableModel->get(logicalIndex);
        if (busman) {
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
                // Переход для отрисовки следующей цифры и немного отступа для просвета между ними
                x += width + 2;

                auto color = schedulerTableModel->linesColorMap[line];

                painter->setBrush(color);

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


            // Рисование индикатора количества ночных смен
            auto x_number_lates = rect.x() + rect.width() - width - vert_indent;
            painter->setPen(Qt::NoPen);

            const auto max_number_late = 4;

            // TODO: дубликат. Аналог есть в scoreinfoboard
            auto number_late = 0;
            for (auto day: busman->workingDays) {
                if (isNight(day)) {
                    number_late++;
                }
            }

            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::yellow);
            painter->setBrush(number_late <= max_number_late ? Qt::yellow : Qt::red);
            painter->drawRect(x_number_lates, rect.y() + vert_indent, width, height);

            painter->setBrush(number_late <= max_number_late ? Qt::green : Qt::red);
            auto indent_led_strip = 2;
            auto height_led_strip = height / (max_number_late + 1) - indent_led_strip / (max_number_late + 1);
            auto y_led_strip = rect.y() + vert_indent + height - height_led_strip;

            // Рисование полос индикатора
            for (int i = 1; i <= number_late; i++) {
                painter->drawRect(x_number_lates, y_led_strip, width, height_led_strip);

                y_led_strip -= indent_led_strip;
                y_led_strip -= height_led_strip;

                if (i >= max_number_late) {
                    break;
                }
            }

            // Рисование количества ночных смен на индикаторе
            painter->setPen(Qt::black);
            painter->drawText(x_number_lates, rect.y() + vert_indent, width, height,
                              Qt::AlignHCenter | Qt::AlignBottom,
                              QString::number(number_late));

            painter->restore();

            return;
        }
    }

    QHeaderView::paintSection(painter, rect, logicalIndex);
}
