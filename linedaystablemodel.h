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
        QList<QPair<Busman::DayKind, Busman::DayKind>> getRow(const QModelIndex& index) throw(std::exception) {
            if (index.row() >= linesList.length()) {
                throw std::logic_error(QString("Выход за пределы списка. (index.row() >= linesList.length() ; %1 >= %2).")
                                       .arg(index.row()).arg(linesList.length()).toStdString());
            }

            return linesList[index.row()];
        }

        // TODO: cpp
        QPair<Busman::DayKind, Busman::DayKind> get(const QModelIndex& index) throw(std::exception) {
            auto row = getRow(index);

            if (index.column() >= row.length()) {
                throw std::logic_error(QString("Выход за пределы списка. (index.column() >= row.length() ; %1 >= %2).")
                                       .arg(index.column()).arg(row.length()).toStdString());
            }

            qDebug() << "get" << row[index.column()] << row[index.column()].first << row[index.column()].second << row[index.column()].first;
            return row[index.column()];
        }

        // TODO: cpp
        Busman::DayKind getLeft(const QModelIndex& index) throw(std::exception) {
//            const QPair<Busman::DayKind, Busman::DayKind>&
            qDebug() << "getLeft" << get(index).first;
            return get(index).first;
        }

        // TODO: cpp
        Busman::DayKind getRight(const QModelIndex& index) throw(std::exception) {
            qDebug() << "getRight" << get(index).second;
            return get(index).second;
        }

        // TODO: cpp
        void set(const QModelIndex& index, QPair<Busman::DayKind, Busman::DayKind> pair) throw(std::exception) {
            // TODO: дубликат
            if (index.row() >= linesList.length()) {
                throw std::logic_error(QString("Выход за пределы списка. (index.row() >= linesList.length() ; %1 >= %2).")
                                       .arg(index.row()).arg(linesList.length()).toStdString());
            }

            // TODO: дубликат
            if (index.column() >= linesList.at(index.row()).length()) {
                throw std::logic_error(QString("Выход за пределы списка. (index.column() >= row.length() ; %1 >= %2).")
                                       .arg(index.column()).arg(linesList.at(index.row()).length()).toStdString());
            }

            linesList[index.row()][index.column()] = pair;
        }

        // TODO: cpp
        void setLeft(const QModelIndex& index, Busman::DayKind day) throw(std::exception) {
            // TODO: дубликат
            if (index.row() >= linesList.length()) {
                throw std::logic_error(QString("Выход за пределы списка. (index.row() >= linesList.length() ; %1 >= %2).")
                                       .arg(index.row()).arg(linesList.length()).toStdString());
            }

            // TODO: дубликат
            if (index.column() >= linesList.at(index.row()).length()) {
                throw std::logic_error(QString("Выход за пределы списка. (index.column() >= row.length() ; %1 >= %2).")
                                       .arg(index.column()).arg(linesList.at(index.row()).length()).toStdString());
            }

            linesList[index.row()][index.column()].first = day;
//            qDebug() << get(index);
//            qDebug() << get(index).first;
//            get(index).first = day;
//            qDebug() << get(index).first;
        }

        // TODO: cpp
        void setRight(const QModelIndex& index, Busman::DayKind day) throw(std::exception) {
            // TODO: дубликат
            if (index.row() >= linesList.length()) {
                throw std::logic_error(QString("Выход за пределы списка. (index.row() >= linesList.length() ; %1 >= %2).")
                                       .arg(index.row()).arg(linesList.length()).toStdString());
            }

            // TODO: дубликат
            if (index.column() >= linesList.at(index.row()).length()) {
                throw std::logic_error(QString("Выход за пределы списка. (index.column() >= row.length() ; %1 >= %2).")
                                       .arg(index.column()).arg(linesList.at(index.row()).length()).toStdString());
            }

            linesList[index.row()][index.column()].second = day;

//            get(index).second = day;
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
        QList<QList<QPair<Busman::DayKind, Busman::DayKind>>> linesList;
};


#endif // LINEDAYSTABLEMODEL_H
