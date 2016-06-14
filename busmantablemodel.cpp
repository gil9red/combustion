#include "busmantablemodel.h"
#include <parserpuzzlefile.h>
#include <QDebug>
#include <QColor>
#include <QLinearGradient>
#include <QFile>


#include <QPainter>


QImage drawBackground(const QImage& im, const QColor& background) {
    QImage image(im.size(), QImage::Format_ARGB32_Premultiplied);
    image.fill(background);

    QPainter p(&image);
    p.drawImage(0, 0, im);

    return image;
}


BusmanTableModel::BusmanTableModel() {
    isVisibleCellText = true;

    QImage sun(":/sun");
    QImage moon(":/moon");

    QColor line1Color = Qt::yellow;
    QColor line2Color = Qt::green;
    QColor line3Color = Qt::cyan;

    linesColorMap[Lines::Line_1] = line1Color;
    linesColorMap[Lines::Line_2] = line2Color;
    linesColorMap[Lines::Line_3] = line3Color;

    linesPairDayKindMap[Lines::Line_1] = QPair<Busman::DayKind, Busman::DayKind> (Busman::DayKind::LINE_1_DAY, Busman::DayKind::LINE_1_NIGHT);
    linesPairDayKindMap[Lines::Line_2] = QPair<Busman::DayKind, Busman::DayKind> (Busman::DayKind::LINE_2_DAY, Busman::DayKind::LINE_2_NIGHT);
    linesPairDayKindMap[Lines::Line_3] = QPair<Busman::DayKind, Busman::DayKind> (Busman::DayKind::LINE_3_DAY, Busman::DayKind::LINE_3_NIGHT);

    stringLineMap["1"] = Lines::Line_1;
    stringLineMap["2"] = Lines::Line_2;
    stringLineMap["3"] = Lines::Line_3;

    lineDaysIconsMap[Busman::DayKind::LINE_1_DAY]   = drawBackground(sun, linesColorMap[Lines::Line_1]);
    lineDaysIconsMap[Busman::DayKind::LINE_1_NIGHT] = drawBackground(moon, linesColorMap[Lines::Line_1]);
    lineDaysIconsMap[Busman::DayKind::LINE_2_DAY]   = drawBackground(sun, linesColorMap[Lines::Line_2]);
    lineDaysIconsMap[Busman::DayKind::LINE_2_NIGHT] = drawBackground(moon, linesColorMap[Lines::Line_2]);
    lineDaysIconsMap[Busman::DayKind::LINE_3_DAY]   = drawBackground(sun, linesColorMap[Lines::Line_3]);
    lineDaysIconsMap[Busman::DayKind::LINE_3_NIGHT] = drawBackground(moon, linesColorMap[Lines::Line_3]);
}

void BusmanTableModel::load(const QString& fileName) throw (std::exception) {
    clear();

    // TODO
    QList<Busman*> newBusmanList = ParserPuzzleFile::parse(fileName/*, valueDescriptionMap*/);
    foreach (Busman *busman, newBusmanList) {
        busmanList.append(busman);
    }

    // Говорим моделе о новых строках и столбцах
    beginInsertRows(QModelIndex(), 0, rowCount() - 1);
    endInsertRows();

    beginInsertColumns(QModelIndex(), 0, columnCount() - 1);
    endInsertColumns();

    sayViewUpdate();
}

void BusmanTableModel::saveAs(const QString& fileName) throw (std::exception) {
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
         out << QString::number(i + 1).rightJustified(2, '0') << "|"
             << busman->busNum << "|"
             // TODO: формат может поменять с строки на другой тип
             << busman->selectLines.rightJustified(3, '0') << "||";

         foreach (QString wishDay, busman->wishesOnSchedule) {
             out << wishDay << "|";
         }
         out << "\n";
    }

    out << "---------------------------------------------------+" << "\n";
}

void BusmanTableModel::clear() {
    qDebug() << "Call clear()";

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

void BusmanTableModel::sayViewUpdate() {
    // Говорим представлению обновиться
    emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}

int BusmanTableModel::rowCount(const QModelIndex &parent) const {
    return busmanList.length();
}

int BusmanTableModel::columnCount(const QModelIndex &parent) const {
    if (busmanList.length() > 0) {
        Busman* busman = busmanList.at(0);
        // Номер автобуса + Выбор линии маршрута + количесто дней в расписании
        return busman->wishesOnSchedule.length();
    }

    return 0;
}

QVariant BusmanTableModel::data(const QModelIndex &index, int role) const {
    const int row = index.row();
    const int column = index.column();

    Busman* busman = busmanList.at(row);
    auto day = busman->workingDays.contains(column) ? busman->workingDays[column] : Busman::DayKind::NONE;

    if (role == Qt::DisplayRole) {
        if (isVisibleCellText) {
            // TODO: дубликат
            return busman->wishesOnSchedule.at(column);
        }

    } else if (role == WishDayRole) {
        // TODO: дубликат
        return busman->wishesOnSchedule.at(column);

        // TODO: хорошо бы в делегате реализовать отрисовку текста
    } else if (role == Qt::ForegroundRole) {
        QString day = busman->wishesOnSchedule.at(column);
        if (day == "XX") {
            return QBrush(Qt::white);
        } else {
            return QBrush(Qt::black);
        }

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
        if (day != Busman::DayKind::NONE) {
            return lineDaysIconsMap[day];
        }
    }

    return QVariant();
}
