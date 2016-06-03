#ifndef BUSMANTABLEMODEL_H
#define BUSMANTABLEMODEL_H

#include <QAbstractTableModel>
#include "busman.h"

class BusmanTableModel : public QAbstractTableModel
{
    public:
        enum COLUMN {
            NUMBER,
            LINES,
        };

    public:
        BusmanTableModel();
        void load(const QString& fileName) throw (std::exception);

//        bool insertItem(Busman* busman) {
//            beginInsertRows(QModelIndex(), busmanList.count(), busmanList.count());
//            busmanList.append(busman);
//            endInsertRows();

////            QModelIndex itemIndex = createIndex(parentItem->IndexOfChild(item), 0, item);
////            QModelIndex itemIndex = createIndex(parentItem->IndexOfChild(item), 0, item);
////                emit dataChanged(itemIndex, itemIndex)
//            emit dataChanged(createIndex(busmanList.count() - 1, 0), createIndex(busmanList.count() - 1, columnCount() - 1));

//            return true;
//        }

    private:
        QList<Busman*> busmanList;

    private:
        int rowCount(const QModelIndex &parent=QModelIndex()) const;
        int columnCount(const QModelIndex &parent=QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role) const;

//        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole )
};

#endif // BUSMANTABLEMODEL_H
