#include "linedaystablemodel.h"
#include <QDebug>


LineDaysTableModel::LineDaysTableModel()
    : QAbstractTableModel() {

    schedulerTableModel = nullptr;
}

int LineDaysTableModel::rowCount(const QModelIndex&) const {
    return linesList.length();
}

int LineDaysTableModel::columnCount(const QModelIndex&) const {
    if (linesList.length() > 0) {
        return linesList[0].length();
    } else {
        return 0;
    }

    if (schedulerTableModel) {
        return schedulerTableModel->columnCount();
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
            return schedulerTableModel->lineDaysIconsMap[pair.first];
        } else {
            return schedulerTableModel->lineDaysIconsMap[pair.second];
        }

    } else if (role == DayKind_Day_Role || role == DayKind_Night_Role) {
        auto pair = linesList.at(row).at(column);

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

    if (schedulerTableModel == 0)
        return;

    for (auto i = 0; i < 3; i++) {
        QList<QPair<DayKind, DayKind>> line;

        for (auto j = 0; j < schedulerTableModel->columnCount(); j++) {
            auto pair = schedulerTableModel->linesPairDayKindMap[(Lines) i];
            line.append(pair);
        }

        linesList.append(line);
    }

    // Говорим моделе о новых строках и столбцах
    beginInsertRows(QModelIndex(), 0, rowCount() - 1);
    endInsertRows();

    beginInsertColumns(QModelIndex(), 0, columnCount() - 1);
    endInsertColumns();

    sayViewUpdate();
}

Lines LineDaysTableModel::getLine(const QModelIndex& index) throw(std::exception) {
    if (index.row() >= 3) {
        throw std::logic_error(QString("Выход за пределы списка. (index.row() >= linesList.length() ; %1 >= %2).")
                               .arg(index.row()).arg(linesList.length()).toStdString());
    }

    // TODO: не очень хорошо такое приведение
    // Линии имеют тот же индекс, что строки, т.е. 0 == Lines::Line_1,
    // 1 == Line_2 и 2 == Line_3. Этим и пользуемся, чтобы узнать к какой линии
    // относится индекс
    return (Lines) index.row();
}

QList<QPair<DayKind, DayKind>> LineDaysTableModel::getRow(const QModelIndex& index) throw(std::exception) {
    if (index.row() >= linesList.length()) {
        throw std::logic_error(QString("Выход за пределы списка. (index.row() >= linesList.length() ; %1 >= %2).")
                               .arg(index.row()).arg(linesList.length()).toStdString());
    }

    return linesList[index.row()];
}

QPair<DayKind, DayKind> LineDaysTableModel::get(const QModelIndex& index) throw(std::exception) {
    auto row = getRow(index);

    if (index.column() >= row.length()) {
        throw std::logic_error(QString("Выход за пределы списка. (index.column() >= row.length() ; %1 >= %2).")
                               .arg(index.column()).arg(row.length()).toStdString());
    }

    return row[index.column()];
}

DayKind LineDaysTableModel::getLeft(const QModelIndex& index) throw(std::exception) {
    return get(index).first;
}

DayKind LineDaysTableModel::getRight(const QModelIndex& index) throw(std::exception) {
    return get(index).second;
}

void LineDaysTableModel::set(const QModelIndex& index, QPair<DayKind, DayKind> pair) throw(std::exception) {
    set(index.row(), index.column(), pair);
}

void LineDaysTableModel::set(int row, int column, QPair<DayKind, DayKind> pair) throw(std::exception) {
    // TODO: дубликат
    if (row >= linesList.length()) {
        throw std::logic_error(QString("Выход за пределы списка. (row >= linesList.length() ; %1 >= %2).")
                               .arg(row).arg(linesList.length()).toStdString());
    }

    // TODO: дубликат
    if (column >= linesList.at(row).length()) {
        throw std::logic_error(QString("Выход за пределы списка. (row >= linesList.at(row).length() ; %1 >= %2).")
                               .arg(column).arg(linesList.at(row).length()).toStdString());
    }

    linesList[row][column] = pair;
}

void LineDaysTableModel::setValue(int row, int column, DayKind day) {
    switch (day) {
        case DayKind::LINE_1_DAY:
        case DayKind::LINE_2_DAY:
        case DayKind::LINE_3_DAY:
            setLeft(row, column, day);
            break;

        case DayKind::LINE_1_NIGHT:
        case DayKind::LINE_2_NIGHT:
        case DayKind::LINE_3_NIGHT:
            setRight(row, column, day);
            break;

        default: {

        }
    }
}

void LineDaysTableModel::setLeft(const QModelIndex& index, DayKind day) throw(std::exception) {
    setLeft(index.row(), index.column(), day);
}

void LineDaysTableModel::setLeft(int row, int column, DayKind day) throw(std::exception) {
    // TODO: дубликат
    if (row >= linesList.length()) {
        throw std::logic_error(QString("Выход за пределы списка. (row >= linesList.length() ; %1 >= %2).")
                               .arg(row).arg(linesList.length()).toStdString());
    }

    // TODO: дубликат
    if (column >= linesList.at(row).length()) {
        throw std::logic_error(QString("Выход за пределы списка. (column >= row.length() ; %1 >= %2).")
                               .arg(column).arg(linesList.at(row).length()).toStdString());
    }

    linesList[row][column].first = day;
}

void LineDaysTableModel::setRight(const QModelIndex& index, DayKind day) throw(std::exception) {
    setRight(index.row(), index.column(), day);
}

void LineDaysTableModel::setRight(int row, int column, DayKind day) throw(std::exception) {
    // TODO: дубликат
    if (row >= linesList.length()) {
        throw std::logic_error(QString("Выход за пределы списка. (row >= linesList.length() ; %1 >= %2).")
                               .arg(row).arg(linesList.length()).toStdString());
    }

    // TODO: дубликат
    if (column >= linesList.at(row).length()) {
        throw std::logic_error(QString("Выход за пределы списка. (column >= row.length() ; %1 >= %2).")
                               .arg(column).arg(linesList.at(row).length()).toStdString());
    }

    linesList[row][column].second = day;
}
