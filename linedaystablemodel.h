#ifndef LINEDAYSTABLEMODEL_H
#define LINEDAYSTABLEMODEL_H


#include <QAbstractTableModel>
#include "scheduler_table_model.h"
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

        Lines getLine(const QModelIndex& index) throw(std::exception);
        QList<QPair<DayKind, DayKind>> getRow(const QModelIndex& index) throw(std::exception);

        QPair<DayKind, DayKind> get(const QModelIndex& index) throw(std::exception);
        DayKind getLeft(const QModelIndex& index) throw(std::exception);
        DayKind getRight(const QModelIndex& index) throw(std::exception);

        void set(const QModelIndex& index, QPair<DayKind, DayKind> pair) throw(std::exception);
        void set(int row, int column, QPair<DayKind, DayKind> pair) throw(std::exception);

        void setValue(int row, int column, DayKind day);

        void setLeft(const QModelIndex& index, DayKind day) throw(std::exception);
        void setLeft(int row, int column, DayKind day) throw(std::exception);

        void setRight(const QModelIndex& index, DayKind day) throw(std::exception);
        void setRight(int row, int column, DayKind day) throw(std::exception);

    public:
        enum UserRole {
            DayKind_Image_Day_Role = Qt::UserRole,
            DayKind_Image_Night_Role = Qt::UserRole + 1,
            DayKind_Day_Role = Qt::UserRole + 2,
            DayKind_Night_Role = Qt::UserRole + 3,
        };

        SchedulerTableModel* schedulerTableModel;

    private:
        // Список, содержащий список пар
        QList<QList<QPair<DayKind, DayKind>>> linesList;
};


#endif // LINEDAYSTABLEMODEL_H
