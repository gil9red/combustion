#ifndef LINEDAYSTABLEMODEL_H
#define LINEDAYSTABLEMODEL_H


#include <QAbstractTableModel>
#include "busmantablemodel.h"


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

    public:
        enum UserRole {
            DayKind_Day_Role = Qt::UserRole,
            DayKind_Night_Role = Qt::UserRole + 1,
        };

        BusmanTableModel* busmanTableModel;

    private:
        // Список, содержащий список пар
        QList<QList<QPair<Busman::DayKind, Busman::DayKind>>> linesList;
};


#endif // LINEDAYSTABLEMODEL_H
