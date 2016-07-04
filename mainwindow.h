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

        void readSettings();
        void writeSettings();

        // Служит продвинутым аналогом сигнала clicked, который раньше отлавливался от таблиц.
        bool eventFilter(QObject* obj, QEvent* event);

    private:
        void updateStates();

        // Функция проверяет возможна ли вставка в таблицу значения.
        // Используется для выделения доступных для вставки ячеек
        // и перед вставкой в ячейки.
        // TODO: разобраться с терминологией проекта -- таблицы снизу и сверху звучат странно
        // Принимает индексы ячейки таблицы сверху и таблицы снизу
        bool isValidSetDayBetweenTables(const QModelIndex& indexTop, const QModelIndex& indexBottom);

        // Проверка индексов ячеек таблиц
        bool isValidIndexes(const QModelIndex& topIndex, const QModelIndex& bottomIndex);

        // TODO: cpp
        // Функция проверяет возможность вставки значения в пределах столбца таблицы
        // расписания. Например, делаем клик на ячейке и в том же столбце кликаем на
        // другую ячейку с такими же линиями маршрута и значение переносится во вторую
        // ячейку.
        // Проверка осуществляется относительно ячейки index1 к ячейке index2
        bool isValidSetDayWithinScheduleTable(const QModelIndex& index1, const QModelIndex& index2) {
            if (!index1.isValid() || !index2.isValid()) {
                return false;
            }

            auto day1 = schedulerTable.model.getDayKind(index1);

            // Линия дня текущего ячейки
            auto line = schedulerTable.model.dayKindsLinesMap[day1];

            // Проверяем, что в ячейке index2 есть такая же линия как index1
            auto busman2 = schedulerTable.model.get(index2);
            bool hasLine = busman2->lines.contains(line);

            // Проверка, что ячейка index2 является рабочим днем, т.е. не "XX"
            bool isWorkingDay2 = busman2->wishesOnSchedule[index2.column()] != "XX";

            // TODO: пока можно внутри столбдца таблицы расписания только в пустые ячейки
            // но условие нужно убрать, когда возможность вставки в занятые ячейки будет сделана
            bool isNoneDay2 = schedulerTable.model.getDayKind(index2) == DayKind::NONE;

            return hasLine && isWorkingDay2 && isNoneDay2;
        }

    private:
        Ui::MainWindow *ui;

        LineDaysTable lineDaysTable;
        SchedulerTable schedulerTable;
        ScoreInfoBoard scoreInfoBoard;

        QWidget* lastClickTable = nullptr;
        QWidget* currClickTable = nullptr;

        QPersistentModelIndex lastClickIndexScheduleTable;
        QPersistentModelIndex currClickIndexScheduleTable;

        // Виджет для показа подсказок работы с таблицами
        QLabel helpManagerLabel;

    protected:
        void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
