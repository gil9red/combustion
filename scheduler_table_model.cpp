#include "scheduler_table_model.h"
#include <parserpuzzlefile.h>
#include <QDebug>
#include <QColor>
#include <QLinearGradient>
#include <QFile>


#include <QPainter>


static const QString BusmanIsNullMessage("busman == nullptr, index: %1, %2.");


QImage drawBackground(const QImage& im, const QColor& background) {
    QImage image(im.size(), QImage::Format_ARGB32_Premultiplied);
    image.fill(background);

    QPainter p(&image);
    p.drawImage(0, 0, im);

    return image;
}

// Функция рисует на копии картинки рамку и возвращает ее
QImage drawFrame(const QImage& im) {
    auto image = im.copy();
    QPainter painter(&image);
    painter.setPen(QPen(Qt::red, 10.0));
    painter.drawRect(image.rect());

    return image;
}


SchedulerTableModel::SchedulerTableModel() {
    isVisibleCellText = true;

    QImage sun(":/sun");
    QImage moon(":/moon");

    QColor line1Color = Qt::yellow;
    QColor line2Color = Qt::green;
    QColor line3Color = Qt::cyan;

    linesColorMap[Lines::Line_1] = line1Color;
    linesColorMap[Lines::Line_2] = line2Color;
    linesColorMap[Lines::Line_3] = line3Color;

    linesPairDayKindMap[Lines::Line_1] = QPair<DayKind, DayKind> (DayKind::LINE_1_DAY, DayKind::LINE_1_NIGHT);
    linesPairDayKindMap[Lines::Line_2] = QPair<DayKind, DayKind> (DayKind::LINE_2_DAY, DayKind::LINE_2_NIGHT);
    linesPairDayKindMap[Lines::Line_3] = QPair<DayKind, DayKind> (DayKind::LINE_3_DAY, DayKind::LINE_3_NIGHT);

    dayKindsLinesMap[DayKind::LINE_1_DAY] = Lines::Line_1;
    dayKindsLinesMap[DayKind::LINE_1_NIGHT] = Lines::Line_1;
    dayKindsLinesMap[DayKind::LINE_2_DAY] = Lines::Line_2;
    dayKindsLinesMap[DayKind::LINE_2_NIGHT] = Lines::Line_2;
    dayKindsLinesMap[DayKind::LINE_3_DAY] = Lines::Line_3;
    dayKindsLinesMap[DayKind::LINE_3_NIGHT] = Lines::Line_3;

    lineDaysIconsMap[DayKind::LINE_1_DAY]   = drawBackground(sun, linesColorMap[Lines::Line_1]);
    lineDaysIconsMap[DayKind::LINE_1_NIGHT] = drawBackground(moon, linesColorMap[Lines::Line_1]);
    lineDaysIconsMap[DayKind::LINE_2_DAY]   = drawBackground(sun, linesColorMap[Lines::Line_2]);
    lineDaysIconsMap[DayKind::LINE_2_NIGHT] = drawBackground(moon, linesColorMap[Lines::Line_2]);
    lineDaysIconsMap[DayKind::LINE_3_DAY]   = drawBackground(sun, linesColorMap[Lines::Line_3]);
    lineDaysIconsMap[DayKind::LINE_3_NIGHT] = drawBackground(moon, linesColorMap[Lines::Line_3]);
}

void SchedulerTableModel::load(const QString& fileName) throw (std::exception) {
    clear();

    foreach (Busman *busman, ParserPuzzleFile::parse(fileName)) {
        busmanList.append(busman);
    }

    // Говорим моделе о новых строках и столбцах
    beginInsertRows(QModelIndex(), 0, rowCount() - 1);
    endInsertRows();

    beginInsertColumns(QModelIndex(), 0, columnCount() - 1);
    endInsertColumns();

    sayViewUpdate();
}

void SchedulerTableModel::saveAs(const QString& fileName) throw (std::exception) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        throw std::logic_error(QString("Не найден файл: %1.").arg(fileName).toStdString());

    QTextStream out(&file);
    out << "GIVEN PUZZLE" << "\n";
    out << "---------------------------------------------------+" << "\n";

    out << "_________|";
    for (int i = 0; i < columnCount(); i++) {
        out << QString::number(i + 1).rightJustified(2, '0') << "|";
    }
    out << "\n";

    for (int i = 0; i < busmanList.size(); i++) {
        Busman* busman = busmanList.at(i);

        QString strLines;
        for (auto line: busman->lines) {
            switch (line) {
                case Lines::Line_1:
                    strLines += "1";
                    break;

                case Lines::Line_2:
                    strLines += "2";
                    break;

                case Lines::Line_3:
                    strLines += "3";
                    break;
            }
        }

        out << QString::number(i + 1).rightJustified(2, '0') << "|"
            << busman->busNum << "|"
            << strLines.rightJustified(3, '0') << "||";

        foreach (QString wishDay, busman->wishesOnSchedule) {
            out << wishDay << "|";
        }
        out << "\n";
    }

    out << "---------------------------------------------------+" << "\n";
}

void SchedulerTableModel::clear() {
    if (rowCount() > 0 || columnCount() > 0) {
        // TODO: посмотреть аналоги в Q*View
        // Говорим моделе сбросить данные
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        endRemoveRows();

        beginRemoveColumns(QModelIndex(), 0, columnCount() - 1);
        endRemoveColumns();

        foreach (Busman* busman, busmanList) {
            delete busman;
        }
        busmanList.clear();
    }
}

void SchedulerTableModel::sayViewUpdate() {
    // Говорим представлению обновиться
    emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}

int SchedulerTableModel::rowCount(const QModelIndex &) const {
    return busmanList.length();
}

int SchedulerTableModel::columnCount(const QModelIndex &) const {
    if (busmanList.length() > 0) {
        Busman* busman = busmanList.at(0);
        return busman->wishesOnSchedule.length();
    }

    return 0;
}

QVariant SchedulerTableModel::data(const QModelIndex &index, int role) const {
    const int row = index.row();
    const int column = index.column();

    Busman* busman = busmanList.at(row);
    auto day = busman->workingDays.at(column);

    if (role == Qt::DisplayRole && isVisibleCellText) {
        return busman->wishesOnSchedule.at(column);

    } else if (role == WishDayRole) {
        return busman->wishesOnSchedule.at(column);

        // TODO: хорошо бы в делегате реализовать отрисовку текста
    } else if (role == Qt::ForegroundRole) {
        QString day = busman->wishesOnSchedule.at(column);
        return QBrush(day == "XX" ? Qt::white : Qt::black);

        // Текст в ячейках располагается по центру
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;

    } else if (role == BusmanRole) {
        QVariant v;
        v.setValue(busman);
        return v;

    } else if (role == DayKindRole) {
        QVariant v;
        v.setValue(day);
        return v;

        // TODO: нормальное название для той таблицы, серьезно!
        // Роль для возврата иконки линии/дня
    } else if (role == DayImageKindRole) {
        if (day != DayKind::NONE) {
            // Проверим, что подряд ночных смен проставлено не больше 3
            // Делаем перебор дней у водителя и ищем те, которые подряд поставлены на ночную
            // смену и если таких наборов больше 3, то следующие дни выделены
            // TODO: для оптимизации нужно искать комбо дней не с первого дня, а с текущего и назад
            // тогда быстрее будет идти поиск, но т.к. таблица тут маленькая оптимизация не будет
            // заметна
            int lateDaySequenceNumber = 0;
            for (int i = 0; i < busman->workingDays.length(); i++) {
                auto day = busman->workingDays[i];

                if (isNight(day)) {
                    lateDaySequenceNumber++;

                    // Если 3 подряд найдено и текущая ячейка относится к тем
                    // лишним ночным сменам, выделяем красной рамкой
                    if (lateDaySequenceNumber > 3 && i == column) {
                        return drawFrame(lineDaysIconsMap[day]);
                    }

                // Последовательность прервана, обнуляем
                } else {
                    lateDaySequenceNumber = 0;
                }
            }

            // Выделяем ячейку дневной смены, если предыдущим от нее днем
            // будет ночная -- две смены подряд это жестко
            // Если текущий день -- дневная смена
            if (isDay(day) && column > 0) {
                // ... а предыдущий день ночная смена
                auto prevDay = busman->workingDays[column - 1];
                if (isNight(prevDay)) {
                    return drawFrame(lineDaysIconsMap[day]);
                }
            }

            return lineDaysIconsMap[day];
        }
    }
    return QVariant();
}

Busman* SchedulerTableModel::get(const QModelIndex& index) const {
    return get(index.row());
}

Busman* SchedulerTableModel::get(int row) const {
    if (row < 0 || row >= busmanList.length()) {
        return nullptr;
    }

    return busmanList.at(row);
}

DayKind SchedulerTableModel::getDayKind(const QModelIndex& index) const throw(std::exception) {
    Busman* busman = get(index);
    if (busman == nullptr) {
        throw std::logic_error(BusmanIsNullMessage.arg(index.row(), index.column()).toStdString());
    }

    auto column = index.column();
    return busman->workingDays.at(column);
}

void SchedulerTableModel::setDayKind(const QModelIndex& index, DayKind day) throw(std::exception) {
    Busman* busman = get(index);
    if (busman == nullptr) {
        throw std::logic_error(BusmanIsNullMessage.arg(index.row(), index.column()).toStdString());
    }

    // Не нужно делать проверку для пустого дня
    if (day != DayKind::NONE) {
        auto line = dayKindsLinesMap[day];
        if (!busman->lines.contains(line)) {
            auto text = QString("Нельзя вставить значение в ячейку: не соответствие линий маршрута. index: %1, %2.").arg(index.row()).arg(index.column());
            throw std::logic_error(text.toStdString());
        }
    }

    busman->workingDays[index.column()] = day;
}

bool SchedulerTableModel::columnInLongRests(int row, int column) const {
    QList<int> indexes;

    // Содержит количество подряд свободных дней
    int noneSequenceNumber = 0;

    // TODO: проверить возможный баг: если ход идет до 14-го дня
    // не встречая рабочих дней, должен выделиться или нет?
    for (int i = 0; i < columnCount(); i++) {
        auto day = getDayKind(index(row, i));

        // Считаем свободные дни
        if (day == DayKind::NONE) {
            noneSequenceNumber++;

            // Свободных дней было от 3 и больше
            if (noneSequenceNumber >= 3 && i >= columnCount() - 1) {
                for (int j = noneSequenceNumber - 1; j >= 0; j--) {
                    indexes << i - j;
                }
            }

        // Наткнулись на рабочий день
        } else {
            // Свободных дней было от 3 и больше
            if (noneSequenceNumber >= 3) {
                for (int j = noneSequenceNumber; j > 0; j--) {
                    indexes << i - j;
                }
            }

            // Последовательность прервана
            noneSequenceNumber = 0;
        }
    }

    return indexes.contains(column);
}

bool SchedulerTableModel::columnInLongRests(const QModelIndex& index) const {
    return columnInLongRests(index.row(), index.column());
}
