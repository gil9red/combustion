#ifndef LINEDAYSTABLE_H
#define LINEDAYSTABLE_H


#include <QTableWidget>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

#include <QAbstractTableModel>
#include "busmantablemodel.h"
#include <QDebug>
#include <QPainter>


namespace LineDaysTableNS {
    class LineDaysTableModel : public QAbstractTableModel
    {
        public:
            LineDaysTableModel() {
                busmanTableModel = 0;
            }

            int rowCount(const QModelIndex &parent=QModelIndex()) const {
//                qDebug() << "row";

//                if (busmanTableModel /*&& linesList.length() > 0 && linesList[0].length() > 0*/) {
//                    // Line 1, 2, 3
//                    return 3;
//                } else {
//                    return 0;
//                }
                return linesList.length();
//                return 3;
            }
            int columnCount(const QModelIndex &parent=QModelIndex()) const {
////                qDebug() << "column";
                if (linesList.length() > 0) {
//                    qDebug() << linesList[0].length();
                    return linesList[0].length();
                } else {
                    return 0;
                }

                if (busmanTableModel /*&& linesList.length() > 0 && linesList[0].length() > 0*/) {
                    return busmanTableModel->columnCount();
                } else {
                    return 0;
                }
            }

            QVariant data(const QModelIndex &index, int role) const {
                const int row = index.row();
                const int column = index.column();

                // TODO: то самое магическое число
                if (column < 2) {
                    return QVariant();
                }

                if (role == DayKind_Day_Role || role == DayKind_Night_Role) {
                    auto pair = linesList.at(row).at(column);

                    if (role == DayKind_Day_Role) {
                        return busmanTableModel->lineDaysIconsMap[pair.first];
                    } else {
                        return busmanTableModel->lineDaysIconsMap[pair.second];
                    }
                }

                return QVariant();
            }
            //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole )

            void clear() {
                qDebug() << "Call clear()";

                // Проверяем наличие по первому подсписку
                if (linesList.length() > 0 && linesList[0].length() > 0) {
                    // TODO: посмотреть аналоги в Q*View
                    // Говорим моделе сбросить данные
                    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
                    endRemoveRows();

                    beginRemoveColumns(QModelIndex(), 0, columnCount() - 1);
                    endRemoveColumns();

                    // Очищение дней в списке линий
                    for (auto subList: linesList)
                        subList.clear();
                }
            }

            void sayViewUpdate() {
                // Говорим представлению обновиться
                emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
            }

            // TODO:
            void reset__() {
                clear();

                if (busmanTableModel == 0)
                    return;

                // Заполнение списка линий
//                for (auto i = 0; i < rowCount(); i++) {
                // TODO:
                for (auto i = 0; i < 3; i++) {
                    qDebug() << i << linesList.length();

                    QList<QPair<Busman::DayKind, Busman::DayKind>> line;

                    // TODO:
                    for (auto j = 0; j < busmanTableModel->columnCount(); j++) {
                        qDebug() << i << linesList.length() << j;
                        auto pair = busmanTableModel->linesPairDayKindMap[(BusmanTableModel::Lines) i];
                        line.append(pair);
                    }

                    linesList.append(line);
//                    qDebug() << "line" << line.length();
                }

//                qDebug() << "\n\n";
//                qDebug() << "_" << linesList[0].length() << linesList.length();
//                qDebug() << "_" << columnCount() << rowCount();
//                qDebug() << "\n\n";

//                for (auto row = 0; row < busmanTableModel->rowCount(); row++) {
//                    for (auto j = 0; j < busmanTableModel->columnCount(); j++) {
////                        auto pair = busmanTableModel->linesPairDayKindMap[(BusmanTableModel::Lines) row];
////                        linesList[row].append(pair);
//                    }
//                }

                // TODO: пока тут бага возникает -- двойное количество строк и почему так понять не смог
                // Говорим моделе о новых строках и столбцах
                beginInsertRows(QModelIndex(), 0, rowCount() - 1);
                endInsertRows();

                beginInsertColumns(QModelIndex(), 0, columnCount() - 1);
                endInsertColumns();

                sayViewUpdate();
            }

        public:
            enum UserRole {
                DayKind_Day_Role = Qt::UserRole,
                DayKind_Night_Role = Qt::UserRole + 1,
            };

            BusmanTableModel* busmanTableModel;

        private:
            // Список, содержащий список пар
            QList<QList<QPair<Busman::DayKind, Busman::DayKind>>> linesList;

//            QList<QPair<Busman::DayKind, Busman::DayKind>> lines_1_list;
//            QList<QPair<Busman::DayKind, Busman::DayKind>> lines_2_list;
//            QList<QPair<Busman::DayKind, Busman::DayKind>> lines_3_list;
    };


    class CellDelegate : public QStyledItemDelegate
    {
        protected:
            void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
                // TODO: магическое число
                if (index.column() >= 2) {
                    QStyleOptionViewItemV4 viewOption = option;

                    painter->save();

                    QRect rect = viewOption.rect;

                    QImage dayImage = index.model()->data(index, LineDaysTableModel::DayKind_Day_Role).value<QImage>();
                    QImage nightImage = index.model()->data(index, LineDaysTableModel::DayKind_Night_Role).value<QImage>();

                    // Отступ между иконками в ячейке
                    auto indent = 2;
                    auto size = qMin(rect.size().width(), rect.size().height()) - indent / 2;
//                    qDebug() << "deleg" << rect.size() << size;
                    dayImage = dayImage.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    nightImage = nightImage.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                    painter->drawImage(rect.x(), rect.y(), dayImage);
                    painter->drawImage(rect.x() + size + indent, rect.y(), nightImage);

                    painter->restore();
                }

                // Здесь дорисовываются стандартные вещи вроде текста, которые берутся из модели
                QStyledItemDelegate::paint(painter, option, index);
            }
    };
}

class LineDaysTable : public QTableView
{
    Q_OBJECT

    public:
        LineDaysTable();

//    private:
//        QStandardItemModel model;
        LineDaysTableNS::LineDaysTableModel model;
};


#endif // LINEDAYSTABLE_H
