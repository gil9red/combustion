#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "busmantablemodel.h"
#include "linedaystable.h"
#include <QResizeEvent>
#include <QItemSelection>
#include "scoreinfoboard.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    public:
        void load(const QString& fileName);

        void open();
        void saveAs();

        void read_settings();
        void write_settings();

    private:
        void selectSchedulerCell(const QModelIndex& index) {
            // Очищение выделения
            tableView.selectionModel()->clearSelection();

            int column = index.column();

            for (int row = 0; row < model.rowCount(); row++) {
                Busman* busman = model.get(row);
                if (busman == nullptr) {
                    qWarning() << "connect(&lineDaysTable, &QTableView::clicked... busman is nullptr, row:" << row;
                    continue;
                }

                // TODO: через модель сделать получение
                if (column >= busman->wishesOnSchedule.length()) {
                    qWarning() << "column >= busman->wishesOnSchedule.length()... column: " << column << ", length: " << busman->wishesOnSchedule.length();
                    continue;
                }

                // Проверяем, что линия выделенной ячейки не совпадает с списком
                // линий маршрута по которым может ездить водитель
                auto line = lineDaysTable.model.getLine(index);
                if (!busman->lines.contains(line)) {
                    continue;
                }

//                auto text = busman->wishesOnSchedule[column];
//
//                // Выделяются только ячейки с RR
//                if (text != "RR") {
//                    continue;
//                }

                auto indexView = model.index(row, column);

                // Выделение ячейки
                tableView.selectionModel()->select(indexView, QItemSelectionModel::Select);
            }
        }

    // TODO: переименовать, подключать не из формы, перенести в срр
    private slots:
        void on_actionSelectSun_triggered() {
            auto topIndex = lineDaysTable.currentIndex();
            auto bottomIndex = tableView.currentIndex();

            // Проверка что ячейки валидные и столбцы выделенных ячеек совпадают
            if (!topIndex.isValid() || !bottomIndex.isValid() || topIndex.column() != bottomIndex.column()) {
                return;
            }

            Busman* busman = model.get(bottomIndex);
            if (busman == nullptr) {
                qWarning() << "busman is nullptr, index:" << bottomIndex;
                return;
            }

            if (lineDaysTable.model.getLeft(topIndex) == DayKind::NONE
                    && lineDaysTable.model.getRight(topIndex) == DayKind::NONE) {
                qWarning() << "Cell is empty";
                return;
            }

            // Берем значение
            auto day = lineDaysTable.model.getLeft(topIndex);
            if (day == DayKind::NONE) {
                qWarning() << "Left value cell is empty";
                return;
            }

            // Убираем значение, которое забрали
            lineDaysTable.model.setLeft(topIndex, DayKind::NONE);

            auto bottomCellDay = model.getDayKind(bottomIndex);
            if (bottomCellDay != DayKind::NONE) {
                // Возврат значения в таблицу выше, которое находилось в ячейке таблице ниже
                // TODO:
                // По дню получаем его строку в таблице выше
                int row = (int) model.dayKindsLinesMap[bottomCellDay];
                lineDaysTable.model.setValue(row, topIndex.column(), bottomCellDay);
            }

            // Вставляем значение в ячейку таблицы
            model.setDayKind(bottomIndex, day);

            // TODO: перерисовывать лучше только изменившуюся ячейку, а не всю таблицу
            model.sayViewUpdate();
            lineDaysTable.model.sayViewUpdate();
        }
        void on_actionSelectMoon_triggered() {
            auto topIndex = lineDaysTable.currentIndex();
            auto bottomIndex = tableView.currentIndex();

            // Проверка что ячейки валидные и столбцы выделенных ячеек совпадают
            if (!topIndex.isValid() || !bottomIndex.isValid() || topIndex.column() != bottomIndex.column()) {
                return;
            }

            Busman* busman = model.get(bottomIndex);
            if (busman == nullptr) {
                qWarning() << "busman is nullptr, index:" << bottomIndex;
                return;
            }

            if (lineDaysTable.model.getLeft(topIndex) == DayKind::NONE
                    && lineDaysTable.model.getRight(topIndex) == DayKind::NONE) {
                qWarning() << "Cell is empty";
                return;
            }

            // Берем значение
            auto day = lineDaysTable.model.getRight(topIndex);
            if (day == DayKind::NONE) {
                qWarning() << "Right value cell is empty";
                return;
            }

            // Убираем значение, которое забрали
            lineDaysTable.model.setRight(topIndex, DayKind::NONE);

            auto bottomCellDay = model.getDayKind(bottomIndex);
            if (bottomCellDay != DayKind::NONE) {
                // Возврат значения в таблицу выше, которое находилось в ячейке таблице ниже
                // TODO:
                // По дню получаем его строку в таблице выше
                int row = (int) model.dayKindsLinesMap[bottomCellDay];
                lineDaysTable.model.setValue(row, topIndex.column(), bottomCellDay);
            }

            // Вставляем значение в ячейку таблицы
            model.setDayKind(bottomIndex, day);

            // TODO: перерисовывать лучше только изменившуюся ячейку, а не всю таблицу
            model.sayViewUpdate();
            lineDaysTable.model.sayViewUpdate();
        }

        void on_actionReturnValue_triggered() {
            auto bottomIndex = tableView.currentIndex();

            auto day = model.getDayKind(bottomIndex);
            if (day == DayKind::NONE) {
                return;
            }

            // TODO: clear
            model.setDayKind(bottomIndex, DayKind::NONE);

            // TODO:
            int row = (int) model.dayKindsLinesMap[day];
            int column = bottomIndex.column();

            lineDaysTable.model.setValue(row, column, day);

            // TODO: перерисовывать лучше только изменившуюся ячейку, а не всю таблицу
            model.sayViewUpdate();
            lineDaysTable.model.sayViewUpdate();
        }

    private:
        Ui::MainWindow *ui;

        LineDaysTable lineDaysTable;

        // TODO: в отдельный виджет как LineDaysTable
        QTableView tableView;
        BusmanTableModel model;

        ScoreInfoBoard scoreInfoBoard;

    protected:
        void closeEvent(QCloseEvent*);

};

#endif // MAINWINDOW_H
