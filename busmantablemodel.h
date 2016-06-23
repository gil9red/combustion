#ifndef BUSMANTABLEMODEL_H
#define BUSMANTABLEMODEL_H

#include <QAbstractTableModel>
#include "busman.h"

#include <QMap>
#include <QBrush>
#include <QTableView>
#include <QDebug>

#include "enums.h"


// TODO: переименовать в SchedulerTableModel
class BusmanTableModel : public QAbstractTableModel
{
    public:
        enum CustomRole {
            BusmanRole = Qt::UserRole,
            WishDayRole = Qt::UserRole + 1,
            DayKindRole = Qt::UserRole + 2,
            DayImageKindRole = Qt::UserRole + 3,
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
        Busman* get(const QModelIndex& index) {
            return get(index.row());
        }

        // TODO: перенести в cpp
        Busman* get(int row) {
            if (row < 0 || row >= busmanList.length()) {
                return nullptr;
            }

            return busmanList.at(row);
        }

        DayKind getDayKind(const QModelIndex& index) throw(std::exception) {
            Busman* busman = get(index);
            if (busman == nullptr) {
                // TODO: дубликат
                throw std::logic_error(QString("busman == nullptr, index: %1, %2.").arg(index.row(), index.column()).toStdString());
            }

            auto column = index.column();
            if (!busman->workingDays.contains(column)) {
                return DayKind::NONE;
            }

            return busman->workingDays[column];
        }

        void setDayKind(const QModelIndex& index, DayKind day) throw(std::exception) {
            Busman* busman = get(index);
            if (busman == nullptr) {
                // TODO: дубликат
                throw std::logic_error(QString("busman == nullptr, index: %1, %2.").arg(index.row(), index.column()).toStdString());
            }

            busman->workingDays[index.column()] = day;
        }

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

#endif // BUSMANTABLEMODEL_H
