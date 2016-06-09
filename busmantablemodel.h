#ifndef BUSMANTABLEMODEL_H
#define BUSMANTABLEMODEL_H

#include <QAbstractTableModel>
#include "busman.h"

#include <QMap>
#include <QBrush>
#include <QTableView>


class BusmanTableModel : public QAbstractTableModel
{
    public:
        enum COLUMN {
            NUMBER,
            LINES,
        };

        enum CustomRole {
            BusmanRole = Qt::UserRole,
            WishDayRole = Qt::UserRole + 1,
        };

        enum Lines {
            Line_1 = 0,
            Line_2,
            Line_3,
        };

    public:
        BusmanTableModel();

        void load(const QString& fileName) throw (std::exception);
        void saveAs(const QString& fileName) throw (std::exception);

        void clear();

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

        int rowCount(const QModelIndex &parent=QModelIndex()) const;
        int columnCount(const QModelIndex &parent=QModelIndex()) const;

        QVariant data(const QModelIndex &index, int role) const;

        void sayViewUpdate();

    private:
        QList<Busman*> busmanList;
        QMap<QString, QBrush> dayKindBackgroundColorMap;
//        QMap<QString, QBrush> dayKindTextColorMap;

    public:
        QMap<QString, QString> valueDescriptionMap;
//        QTableView *view;

        // TODO: сделать private обернуть в функции set/get + при изменении значения
        // уведомлять об этом представление
        // Показывать текст ячеек с расписанием: XX, 00, DD и т.п.
        bool isVisibleCellText;

        // Словарь содержит иконки для обозначения цветом линии (1, 2 и 3) и рабочие смены (день и ночь),
        // по перечислению Busman::DayKind
        QMap<Busman::DayKind, QImage> lineDaysIconsMap;

        // Словарь содержит описание цвета для линий.
        QMap<Lines, QColor> linesColorMap;

        QMap<Lines, QPair<Busman::DayKind, Busman::DayKind>> linesPairDayKindMap;

//        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole )
};

#endif // BUSMANTABLEMODEL_H
