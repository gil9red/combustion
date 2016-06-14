#include "linedaystablemodel.h"
#include <QDebug>


LineDaysTableModel::LineDaysTableModel() {
    busmanTableModel = 0;
}

int LineDaysTableModel::rowCount(const QModelIndex &parent) const {
    return linesList.length();
}

int LineDaysTableModel::columnCount(const QModelIndex &parent) const {
    if (linesList.length() > 0) {
        return linesList[0].length();
    } else {
        return 0;
    }

    if (busmanTableModel) {
        return busmanTableModel->columnCount();
    } else {
        return 0;
    }
}

QVariant LineDaysTableModel::data(const QModelIndex &index, int role) const {
    const int row = index.row();
    const int column = index.column();

    if (role == DayKind_Image_Day_Role || role == DayKind_Image_Night_Role) {
        auto pair = linesList.at(row).at(column);

        if (role == DayKind_Image_Day_Role) {
            return busmanTableModel->lineDaysIconsMap[pair.first];
        } else {
            return busmanTableModel->lineDaysIconsMap[pair.second];
        }

    } else if (role == DayKind_Day_Role || role == DayKind_Night_Role) {
        auto pair = linesList.at(row).at(column);
//        qDebug() << pair;

        QVariant v;
        if (role == DayKind_Day_Role) {
            v.setValue(pair.first);
        } else {
            v.setValue(pair.second);
        }

        return v;
    }

    return QVariant();
}

QVariant LineDaysTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return QString("День %1").arg(section + 1);
    }

    return QVariant();
}

void LineDaysTableModel::clear() {
    qDebug() << "Call clear()" ;

    // Проверяем наличие по первому подсписку
    if (linesList.length() > 0 && linesList[0].length() > 0) {
        // TODO: посмотреть аналоги в Q*View
        // Говорим моделе сбросить данные
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        endRemoveRows();

        beginRemoveColumns(QModelIndex(), 0, columnCount() - 1);
        endRemoveColumns();

        // Очищение дней в списке линий
        for (auto subList: linesList) {
            subList.clear();
        }
        linesList.clear();
    }
}

void LineDaysTableModel::sayViewUpdate() {
    // Говорим представлению обновиться
    emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}

// TODO:
void LineDaysTableModel::reset__() {
    clear();

    if (busmanTableModel == 0)
        return;

    // TODO:
    for (auto i = 0; i < 3; i++) {
//        qDebug() << i << linesList.length();

        QList<QPair<Busman::DayKind, Busman::DayKind>> line;

        QDebug deb = qDebug();

        // TODO:
        for (auto j = 0; j < busmanTableModel->columnCount(); j++) {
//            qDebug() << i << linesList.length() << j;
            auto pair = busmanTableModel->linesPairDayKindMap[(BusmanTableModel::Lines) i];
            line.append(pair);
            deb << pair;
        }

        linesList.append(line);
    }

    // TODO: пока тут бага возникает -- двойное количество строк и почему так понять не смог
    // Говорим моделе о новых строках и столбцах
    beginInsertRows(QModelIndex(), 0, rowCount() - 1);
    endInsertRows();

    beginInsertColumns(QModelIndex(), 0, columnCount() - 1);
    endInsertColumns();

    sayViewUpdate();
}
