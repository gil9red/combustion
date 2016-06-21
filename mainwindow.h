#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "linedaystable.h"
#include <QResizeEvent>
#include <QItemSelection>
#include "scoreinfoboard.h"
#include "schedulertable.h"


namespace Ui {
    class MainWindow;
}

// TODO: повторить методы модели в табличное представление
// поиск "schedulerTable.model" покажет все методы,
// которые нужно перенести

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
        void updateStates();

        // Функция проверяет возможна ли вставка в таблицу значения.
        // Используется для выделения доступных для вставки ячеек
        // и перед вставкой в ячейки.
        // TODO: разобраться с терминологией проекта -- таблицы снизу и сверху звучат странно
        // Принимает индексы ячейки таблицы сверху и таблицы снизу
        bool isValidSetDay(const QModelIndex& indexTop, const QModelIndex& indexBottom) {
            const auto column = indexBottom.column();

            Busman* busman = schedulerTable.model.get(indexBottom);
            if (busman == nullptr) {
                qWarning() << "connect(&lineDaysTable, &QTableView::clicked... busman is nullptr, row:" << indexBottom.row();
                return false;
            }

            // TODO: через модель сделать получение
            if (column >= busman->wishesOnSchedule.length()) {
                qWarning() << "column >= busman->wishesOnSchedule.length()... column: " << column << ", length: " << busman->wishesOnSchedule.length();
                return false;
            }

            // Проверяем, что линия выделенной ячейки не совпадает с списком
            // линий маршрута по которым может ездить водитель
            auto line = lineDaysTable.model.getLine(indexTop);
            if (!busman->lines.contains(line)) {
                return false;
            }

            auto text = busman->wishesOnSchedule[column];

            // TODO: проверить
            // В эти дни у водителей гарантированные выходные
            if (text == "XX") {
                return false;
            }

            return true;
        }

        // Проверка индексов ячеек таблиц
        bool isValidIndexes(const QModelIndex& topIndex, const QModelIndex& bottomIndex) {
            // Проверка валидности индексов
            bool isValid = topIndex.isValid() && bottomIndex.isValid();

            // Проверка того, что столбцы одинаковые
            isValid = isValid && topIndex.column() == bottomIndex.column();
            return isValid;
        }

        // Функция выделяет в таблице расписания доступные для установки ячейки
        void selectSchedulerCell(const QModelIndex& index) {
            // Очищение выделения
            schedulerTable.selectionModel()->clearSelection();

            int column = index.column();

            for (int row = 0; row < schedulerTable.model.rowCount(); row++) {
                auto indexView = schedulerTable.model.index(row, column);

                // Выделение ячейки
                if (isValidSetDay(index, indexView)) {
                    schedulerTable.selectionModel()->select(indexView, QItemSelectionModel::Select);
                }
            }
        }

    // TODO: переименовать, подключать не из формы, перенести в срр
    private slots:
        void on_actionSelectSun_triggered() {
            auto topIndex = lineDaysTable.currentIndex();
            auto bottomIndex = schedulerTable.currentIndex();

            if (!isValidIndexes(topIndex, bottomIndex)) {
                return;
            }

            Busman* busman = schedulerTable.model.get(bottomIndex);
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

            auto bottomCellDay = schedulerTable.model.getDayKind(bottomIndex);
            if (bottomCellDay != DayKind::NONE) {
                // Возврат значения в таблицу выше, которое находилось в ячейке таблице ниже
                // TODO:
                // По дню получаем его строку в таблице выше
                int row = (int) schedulerTable.model.dayKindsLinesMap[bottomCellDay];
                lineDaysTable.model.setValue(row, topIndex.column(), bottomCellDay);
            }

            // Вставляем значение в ячейку таблицы
            schedulerTable.model.setDayKind(bottomIndex, day);

            // TODO: перерисовывать лучше только изменившуюся ячейку, а не всю таблицу
            schedulerTable.model.sayViewUpdate();
            lineDaysTable.model.sayViewUpdate();
        }
        void on_actionSelectMoon_triggered() {
            auto topIndex = lineDaysTable.currentIndex();
            auto bottomIndex = schedulerTable.currentIndex();

            if (!isValidIndexes(topIndex, bottomIndex)) {
                return;
            }

            Busman* busman = schedulerTable.model.get(bottomIndex);
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

            auto bottomCellDay = schedulerTable.model.getDayKind(bottomIndex);
            if (bottomCellDay != DayKind::NONE) {
                // Возврат значения в таблицу выше, которое находилось в ячейке таблице ниже
                // TODO:
                // По дню получаем его строку в таблице выше
                int row = (int) schedulerTable.model.dayKindsLinesMap[bottomCellDay];
                lineDaysTable.model.setValue(row, topIndex.column(), bottomCellDay);
            }

            // Вставляем значение в ячейку таблицы
            schedulerTable.model.setDayKind(bottomIndex, day);

            // TODO: перерисовывать лучше только изменившуюся ячейку, а не всю таблицу
            schedulerTable.model.sayViewUpdate();
            lineDaysTable.model.sayViewUpdate();
        }

        void on_actionReturnValue_triggered() {
            auto bottomIndex = schedulerTable.currentIndex();

            auto day = schedulerTable.model.getDayKind(bottomIndex);
            if (day == DayKind::NONE) {
                return;
            }

            // TODO: clear
            schedulerTable.model.setDayKind(bottomIndex, DayKind::NONE);

            // TODO:
            int row = (int) schedulerTable.model.dayKindsLinesMap[day];
            int column = bottomIndex.column();

            lineDaysTable.model.setValue(row, column, day);

            // TODO: перерисовывать лучше только изменившуюся ячейку, а не всю таблицу
            schedulerTable.model.sayViewUpdate();
            lineDaysTable.model.sayViewUpdate();
        }

    private:
        Ui::MainWindow *ui;

        LineDaysTable lineDaysTable;
        SchedulerTable schedulerTable;
        ScoreInfoBoard scoreInfoBoard;

    protected:
        void closeEvent(QCloseEvent*);

};

#endif // MAINWINDOW_H
