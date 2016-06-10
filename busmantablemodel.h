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

        int rowCount(const QModelIndex &parent=QModelIndex()) const;
        int columnCount(const QModelIndex &parent=QModelIndex()) const;

        QVariant data(const QModelIndex &index, int role) const;

        void sayViewUpdate();

        // TODO: перенести в cpp
        Busman* get(int row) {
            if (row < 0 || row >= busmanList.length())
                return nullptr;

            return busmanList.at(row);
        }

    private:
        QList<Busman*> busmanList;
        QMap<QString, QBrush> dayKindBackgroundColorMap;

    public:
        // TODO:
//        QMap<QString, QString> valueDescriptionMap;

        // TODO: сделать private обернуть в функции set/get + при изменении значения
        // уведомлять об этом представление
        // Показывать текст ячеек с расписанием: XX, 00, DD и т.п.
        bool isVisibleCellText;

        // Словарь содержит иконки для обозначения цветом линии (1, 2 и 3) и рабочие смены (день и ночь),
        // по перечислению Busman::DayKind
        QMap<Busman::DayKind, QImage> lineDaysIconsMap;

        // Словарь содержит описание цвета для линий.
        QMap<Lines, QColor> linesColorMap;

        QMap<QString, Lines> stringLineMap;

        QMap<Lines, QPair<Busman::DayKind, Busman::DayKind>> linesPairDayKindMap;
};

#endif // BUSMANTABLEMODEL_H
