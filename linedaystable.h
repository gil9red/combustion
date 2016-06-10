#ifndef LINEDAYSTABLE_H
#define LINEDAYSTABLE_H


#include <QTableWidget>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

#include <QAbstractTableModel>
#include "busmantablemodel.h"
#include <QDebug>
#include <QPainter>
#include <QHeaderView>


namespace LineDaysTableNS {
    // TODO: перенести определение в cpp
    class LineDaysTableModel : public QAbstractTableModel
    {
        public:
            LineDaysTableModel() {
                busmanTableModel = 0;
            }

            int rowCount(const QModelIndex &parent=QModelIndex()) const {
                return linesList.length();
            }
            int columnCount(const QModelIndex &parent=QModelIndex()) const {
                if (linesList.length() > 0) {
                    return linesList[0].length();
                } else {
                    return 0;
                }

                if (busmanTableModel) {
                    return busmanTableModel->columnCount();
                } else {
                    return 0;
                }
            }

            QVariant data(const QModelIndex &index, int role) const {
                const int row = index.row();
                const int column = index.column();

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

            QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const {
                // TODO: магичкеское число
                if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
                    // TODO: магичкеское число
                    return QString("День %1").arg(section + 1);
                }

                return QVariant();
            }

            void clear() {
                qDebug() << "Call clear()" ;

                // Проверяем наличие по первому подсписку
                if (linesList.length() > 0 && linesList[0].length() > 0) {
                    // TODO: посмотреть аналоги в Q*View
                    // Говорим моделе сбросить данные
                    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
                    endRemoveRows();

                    beginRemoveColumns(QModelIndex(), 0, columnCount() - 1);
                    endRemoveColumns();

                    // Очищение дней в списке линий
                    for (auto subList: linesList) {
//                        subList.clear();
                        linesList.clear();
                    }
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
                }

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
    };


    // TODO: перенести определение в cpp
    class CellDelegate : public QStyledItemDelegate
    {
        protected:
            void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
                painter->save();

                QRect rect = option.rect;

                QImage dayImage = index.model()->data(index, LineDaysTableModel::DayKind_Day_Role).value<QImage>();
                QImage nightImage = index.model()->data(index, LineDaysTableModel::DayKind_Night_Role).value<QImage>();

                // Отступ между иконками в ячейке
                auto indent = 2;
                auto size = qMin(rect.size().width(), rect.size().height()) - indent / 2;
                dayImage = dayImage.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                nightImage = nightImage.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                painter->drawImage(rect.x(), rect.y(), dayImage);
                painter->drawImage(rect.x() + size + indent, rect.y(), nightImage);

                painter->restore();

                // Цвет выделения полупрозрачный, чтобы были видно что в ячейке
                QStyleOptionViewItem itemOption(option);
                initStyleOption(&itemOption, index);

                if ((itemOption.state & QStyle::State_Selected) && (itemOption.state & QStyle::State_Active)) {
                    auto color = itemOption.palette.color(QPalette::Highlight);
                    color.setAlpha(180);
                    itemOption.palette.setColor(QPalette::Highlight, color);
                }

                QStyledItemDelegate::paint(painter, itemOption, index);
            }
    };


    // TODO: в cpp
    class VerticalHeaderView: public QHeaderView
    {
        Q_OBJECT

        public:
            VerticalHeaderView(QWidget* parent = Q_NULLPTR) : QHeaderView(Qt::Vertical, parent) {

            }

        protected:
            // TODO: вынести в cpp
            void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const {
//                BusmanTableModel* busmanTableModel = dynamic_cast<BusmanTableModel*> (model());
//                if (busmanTableModel != nullptr) {
//                    Busman* busman = busmanTableModel->get(logicalIndex);
//                    if (busman != nullptr) {
//                        QStyleOptionHeader option;
//                        initStyleOption(&option);
//                        option.rect = rect;

//                        // Отрисовка ячейки и затемнение фона, чтобы было отличие от ячеек таблицы
//                        style()->drawControl(QStyle::CE_HeaderSection, &option, painter);
//                        painter->save();
//                        painter->setPen(Qt::NoPen);
//                        painter->setBrush(QColor(0, 0, 0, 20));
//                        painter->drawRect(option.rect);
//                        painter->restore();


//                        const int text_indent = 10;
//                        const int vert_indent = 6;
//                        int x = rect.x() + text_indent;
//                        int y = rect.y() + vert_indent;
//                        int width = 15; // Ширина, которая дается для отрисовки символа в заголовке
//                        int height = rect.height() - vert_indent * 2;

//                        painter->save();

//                        auto font = painter->font();
//                        font.setBold(true);
//                        painter->setFont(font);
//                        painter->drawText(x, y, width, height, Qt::AlignLeft | Qt::AlignVCenter, busman->busNum);

//                        painter->setPen(Qt::black);

//                        // Немного отступа после номера автобуса
//                        x += 5;
//                        for (auto strLine: busman->selectLines) {
//                            // Переход для отрисовки следующей цифры и немного отступа для просвета между ними
//                            x += width + 2;

//                            auto line = busmanTableModel->stringLineMap[strLine];
//                            auto color = busmanTableModel->linesColorMap[line];

//                            painter->setBrush(color);

//                            // Отрисовка рамки и текста в ней
//                            painter->drawRect(x, y, width, height);
//                            painter->drawText(x, y, width, height, Qt::AlignCenter, strLine);
//                        }

//                        painter->restore();

//                        return;
//                    }
//                }

                // TODO: добавить проверку как выше
                LineDaysTableModel* lineDaysTableModel = dynamic_cast<LineDaysTableModel*> (model());
                auto busmanTableModel = lineDaysTableModel->busmanTableModel;

                QStyleOptionHeader option;
                initStyleOption(&option);
                option.rect = rect;

                // Отрисовка ячейки и затемнение фона, чтобы было отличие от ячеек таблицы
                style()->drawControl(QStyle::CE_HeaderSection, &option, painter);
                painter->save();
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor(0, 0, 0, 20));
                painter->drawRect(option.rect);
                painter->restore();

                auto font = painter->font();
                font.setBold(true);
                painter->setFont(font);
                painter->setPen(Qt::black);

                const int text_indent = 10;
                const int vert_indent = 6;
                int x = rect.x() + text_indent;
                int y = rect.y() + vert_indent;
                int width = rect.width() - text_indent * 2;
                int height = rect.height() - vert_indent * 2;

                // TODO: неочень хорошо выглядит это получение цвета линии
                auto line = busmanTableModel->stringLineMap[QString::number(logicalIndex + 1)];
                auto color = busmanTableModel->linesColorMap[line];
                painter->setBrush(color);

                // Отрисовка рамки и текста в ней
                painter->drawRect(x, y, width, height);
                painter->drawText(x, y, width, height, Qt::AlignCenter, QString("Линия %1").arg(logicalIndex + 1));

//                QHeaderView::paintSection(painter, rect, logicalIndex);
            }
    };
}

class LineDaysTable : public QTableView
{
    Q_OBJECT

    public:
        LineDaysTable();

        // TODO:
        LineDaysTableNS::LineDaysTableModel model;
};


#endif // LINEDAYSTABLE_H
