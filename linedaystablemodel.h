#ifndef LINEDAYSTABLEMODEL_H
#define LINEDAYSTABLEMODEL_H


#include <QAbstractTableModel>
#include "busmantablemodel.h"
#include <QDebug>


class LineDaysTableModel : public QAbstractTableModel
{
    public:
        LineDaysTableModel();

        int rowCount(const QModelIndex &parent=QModelIndex()) const;
        int columnCount(const QModelIndex &parent=QModelIndex()) const;

        QVariant data(const QModelIndex &index, int role) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        void clear();

        void sayViewUpdate();

        // TODO:
        void reset__();

        // TODO: cpp
        Lines getLine(const QModelIndex& index) throw(std::exception) {
            if (index.row() >= 3) {
                throw std::logic_error(QString("Выход за пределы списка. (index.row() >= linesList.length() ; %1 >= %2).")
                                       .arg(index.row()).arg(linesList.length()).toStdString());
            }

            // TODO: не очень хорошо
            // Линии имеют тот же индекс, что строки, т.е. 0 == Lines::Line_1,
            // 1 == Line_2 и 2 == Line_3. Этим и пользуемся, чтобы узнать к какой линии
            // относится индекс
            return (Lines) index.row();
        }

        // TODO: cpp
        QList<QPair<DayKind, DayKind>> getRow(const QModelIndex& index) throw(std::exception) {
            if (index.row() >= linesList.length()) {
                throw std::logic_error(QString("Выход за пределы списка. (index.row() >= linesList.length() ; %1 >= %2).")
                                       .arg(index.row()).arg(linesList.length()).toStdString());
            }

            return linesList[index.row()];
        }

        // TODO: cpp
        QPair<DayKind, DayKind> get(const QModelIndex& index) throw(std::exception) {
            auto row = getRow(index);

            if (index.column() >= row.length()) {
                throw std::logic_error(QString("Выход за пределы списка. (index.column() >= row.length() ; %1 >= %2).")
                                       .arg(index.column()).arg(row.length()).toStdString());
            }

            return row[index.column()];
        }

        // TODO: cpp
        DayKind getLeft(const QModelIndex& index) throw(std::exception) {
            return get(index).first;
        }

        // TODO: cpp
        DayKind getRight(const QModelIndex& index) throw(std::exception) {
            return get(index).second;
        }

        // TODO: cpp
        void set(const QModelIndex& index, QPair<DayKind, DayKind> pair) throw(std::exception) {
            set(index.row(), index.column(), pair);
        }

        // TODO: cpp
        void set(int row, int column, QPair<DayKind, DayKind> pair) throw(std::exception) {
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

        void setValue(int row, int column, DayKind day) {
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

        // TODO: cpp
        void setLeft(const QModelIndex& index, DayKind day) throw(std::exception) {
            setLeft(index.row(), index.column(), day);
        }

        // TODO: cpp
        void setLeft(int row, int column, DayKind day) throw(std::exception) {
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

        // TODO: cpp
        void setRight(const QModelIndex& index, DayKind day) throw(std::exception) {
            setRight(index.row(), index.column(), day);
        }

        // TODO: cpp
        void setRight(int row, int column, DayKind day) throw(std::exception) {
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

    public:
        enum UserRole {
            DayKind_Image_Day_Role = Qt::UserRole,
            DayKind_Image_Night_Role = Qt::UserRole + 1,
            DayKind_Day_Role = Qt::UserRole + 2,
            DayKind_Night_Role = Qt::UserRole + 3,
        };

        BusmanTableModel* busmanTableModel;

    private:
        // Список, содержащий список пар
        QList<QList<QPair<DayKind, DayKind>>> linesList;
};


#endif // LINEDAYSTABLEMODEL_H
