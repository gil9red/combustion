#ifndef SchedulerTableMODEL_H
#define SchedulerTableMODEL_H

#include <QAbstractTableModel>
#include "busman.h"

#include <QMap>
#include <QBrush>
#include <QTableView>
#include <QDebug>

#include "enums.h"


class SchedulerTableModel : public QAbstractTableModel
{
    public:
        enum CustomRole {
            BusmanRole = Qt::UserRole,
            WishDayRole = Qt::UserRole + 1,
            DayKindRole = Qt::UserRole + 2,
            DayImageKindRole = Qt::UserRole + 3,
        };

    public:
        SchedulerTableModel();

        void load(const QString& fileName) throw (std::exception);
        void saveAs(const QString& fileName) throw (std::exception);

        void clear();

        int rowCount(const QModelIndex &parent=QModelIndex()) const;
        int columnCount(const QModelIndex &parent=QModelIndex()) const;

        QVariant data(const QModelIndex &index, int role) const;

        void sayViewUpdate();

        Busman* get(const QModelIndex& index) const;
        Busman* get(int row) const;

        DayKind getDayKind(const QModelIndex& index) const throw(std::exception);
        void setDayKind(const QModelIndex& index, DayKind day) throw(std::exception);

        // Возвращает true, если указанный день попадает в последовательность
        // в которой у водителя перед сменой есть три и более свободных дня
        bool columnInLongRests(int row, int column) const;
        bool columnInLongRests(const QModelIndex& index) const;

    private:
        QList<Busman*> busmanList;
        QMap<QString, QBrush> dayKindBackgroundColorMap;

    public:
        // TODO: сделать private обернуть в функции set/get + при изменении значения
        // уведомлять об этом представление
        // Показывать текст ячеек с расписанием: XX, 00, DD и т.п.
        bool isVisibleCellText;

        // Словарь содержит иконки для обозначения цветом линии (1, 2 и 3) и рабочие смены (день и ночь),
        // по перечислению DayKind
        QMap<DayKind, QImage> lineDaysIconsMap;

        // Словарь содержит описание цвета для линий.
        QMap<Lines, QColor> linesColorMap;

        QMap<Lines, QPair<DayKind, DayKind>> linesPairDayKindMap;
        QMap<DayKind, Lines> dayKindsLinesMap;
};

#endif // SchedulerTableMODEL_H
