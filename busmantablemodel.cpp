#include "busmantablemodel.h"
#include <parserpuzzlefile.h>
#include <QDebug>


BusmanTableModel::BusmanTableModel() {

}

void BusmanTableModel::load(const QString& fileName) throw (std::exception) {
    foreach (Busman* busman, busmanList) {
        delete busman;
    }
    busmanList.clear();

//    busmanList = ParserPuzzleFile::parse(fileName);
//    qDebug() << busmanList.size();
//    foreach (Busman *busman, busmanList) {
//        qDebug() << busman->busNum << busman->selectLines;
//    }

    QList<Busman*> newBusmanList = ParserPuzzleFile::parse(fileName);
    qDebug() << newBusmanList.size();
    foreach (Busman *busman, newBusmanList) {
//        insertItem(busman);
        busmanList.append(busman);
        qDebug() << busman->busNum << busman->selectLines;
    }

    // Говорим моделе о новых строках и столбцах
    beginInsertRows(QModelIndex(), 0, rowCount() - 1);
    endInsertRows();

    beginInsertColumns(QModelIndex(), 0, columnCount() - 1);
    endInsertColumns();

    // Говорим представлению обновиться
    emit dataChanged(createIndex(0, 0), createIndex(busmanList.count() - 1, columnCount() - 1));

//    try {
//        busmanList = ParserPuzzleFile::parse(fileName);
//        qDebug() << busmanList.size();
//        foreach (Busman *busman, busmanList) {
//            qDebug() << busman->busNum << busman->selectLines;
//        }

//    } catch(std::exception& e) {
//        qCritical() << e.what();
//    }
}

int BusmanTableModel::rowCount(const QModelIndex &parent) const {
    return busmanList.size();
}

int BusmanTableModel::columnCount(const QModelIndex &parent) const {
    if (busmanList.size() > 0) {
        Busman* busman = busmanList.at(0);
        // Номер автобуса + Выбор линии маршрута + количесто дней в расписании
        return 2 + busman->wishesOnSchedule.size();
    }

    return 0;
}

QVariant BusmanTableModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        int row = index.row();
        Busman* busman = busmanList.at(row);

        int column = index.column();
        switch (column) {
            case COLUMN::NUMBER:
                return busman->busNum;

            case COLUMN::LINES:
                return busman->selectLines;

            default:
                // Минус 2: Номер автобуса + Выбор линии маршрута
                return busman->wishesOnSchedule.at(column - 2);
        }

//        QString unswer = QString("row = ") + QString::number(index.row()) + "  col = " + QString::number(index.column());
//        // строкой выше мы формируем ответ. QString::number преобразует число в текст
//        return QVariant(unswer);
    }
    return QVariant();
}
