#include "busmantablemodel.h"
#include <parserpuzzlefile.h>
#include <QDebug>
#include <QColor>
#include <QLinearGradient>


BusmanTableModel::BusmanTableModel() {
    isVisibleCellText = true;

//    view = 0;

//    dayKindBackgroundColorMap["RR"] = QBrush(Qt::gray);

//////    QGradient gradient(0.0, 0.0, 0.0, 1.0);
//////    gradient.setColorAt(0, Qt::black);
//////    gradient.setColorAt(1, Qt::white);
////    QLinearGradient gradient(QPointF(0, 0), QPointF(1, 1));
////    gradient.setSpread(QGradient::RepeatSpread);
////    gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
////    gradient.setColorAt(0, Qt::black);
////    gradient.setColorAt(1, Qt::white);
////    dayKindColorMap["XX"] = QBrush(gradient);
//    dayKindBackgroundColorMap["XX"] = QBrush(Qt::black);

////    dayKindTextColorMap["XX"] = QBrush(Qt::white);
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

    QList<Busman*> newBusmanList = ParserPuzzleFile::parse(fileName, valueDescriptionMap);
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

    sayViewUpdate();

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

void BusmanTableModel::sayViewUpdate() {
    // Говорим представлению обновиться
    emit dataChanged(createIndex(0, 0), createIndex(busmanList.count() - 1, columnCount() - 1));
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
//    qDebug() << view->visualRect(index).size();

    const int row = index.row();
    const int column = index.column();

    Busman* busman = busmanList.at(row);

    if (role == Qt::DisplayRole) {
        if (column == COLUMN::NUMBER) {
            return busman->busNum;

        } else if (column == COLUMN::LINES) {
            return busman->selectLines;

        } else {
            if (isVisibleCellText) {
                // TODO: дубликат
                // Минус 2: Номер автобуса + Выбор линии маршрута
                return busman->wishesOnSchedule.at(column - 2);
            }
        }

    } else if (role == WishDayRole) {
        switch (column) {
            case COLUMN::NUMBER:
            case COLUMN::LINES:
                break;

            default: {
                // TODO: дубликат
                // Минус 2: Номер автобуса + Выбор линии маршрута
                return busman->wishesOnSchedule.at(column - 2);
            }
        }

        // TODO: хорошо бы в делегате реализовать отрисовку текста
    } else if (role == Qt::ForegroundRole) {
        switch (column) {
            case COLUMN::NUMBER:
            case COLUMN::LINES:
                break;

            default: {
                // Минус 2: Номер автобуса + Выбор линии маршрута
                QString day = busman->wishesOnSchedule.at(column - 2);
                if (day == "XX") {
                    return QBrush(Qt::white);
                } else {
                    return QBrush(Qt::black);
                }
            }
        }

    // Текст в ячейках располагается по центру
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;

    } else if (role == BusmanRole) {
        QVariant v;
        v.setValue(busman);
        return v;
    }

//    switch (role) {
//        case Qt::DisplayRole:
//        case WishDayRole: {
//            switch (column) {
//                // TODO: NUMBER и LINES не относятся к расписанию (т.е. к WishDayRole)
//                case COLUMN::NUMBER:
//                    return busman->busNum;

//                case COLUMN::LINES:
//                    return busman->selectLines;

//                default:
//                    // Минус 2: Номер автобуса + Выбор линии маршрута
//                    return busman->wishesOnSchedule.at(column - 2);
//            }
//            break;
//        }

////        case Qt::BackgroundRole: {
////            switch (column) {
////                case COLUMN::NUMBER:
////                case COLUMN::LINES:
////                    break;

////                default:
////                    // Минус 2: Номер автобуса + Выбор линии маршрута
////                    QString day = busman->wishesOnSchedule.at(column - 2);
////                    if (dayKindBackgroundColorMap.contains(day)) {
////                        return dayKindBackgroundColorMap[day];
////                    }
////                    break;
////            }
////            break;
////        }

//        // TODO: хорошо бы в делегате реализовать отрисовку текста
//        case Qt::ForegroundRole: {
//            switch (column) {
//                case COLUMN::NUMBER:
//                case COLUMN::LINES:
//                    break;

//                default:
//                    // Минус 2: Номер автобуса + Выбор линии маршрута
//                    QString day = busman->wishesOnSchedule.at(column - 2);
//                    if (day == "XX") {
//                        return QBrush(Qt::white);
//                    } else {
//                        return QBrush(Qt::black);
//                    }
//            }
//            break;
//        }

//        case BusmanRole:
//            QVariant v;
//            v.setValue(busman);
//            return v;
//    }

    return QVariant();
}
