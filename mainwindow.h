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

        // Служит продвинутым аналогом сигнала clicked, который раньше отлавливался от таблиц.
        bool eventFilter(QObject* obj, QEvent* event);

    private:
        void updateStates();

        // Функция проверяет возможна ли вставка в таблицу значения.
        // Используется для выделения доступных для вставки ячеек
        // и перед вставкой в ячейки.
        // TODO: разобраться с терминологией проекта -- таблицы снизу и сверху звучат странно
        // Принимает индексы ячейки таблицы сверху и таблицы снизу
        bool isValidSetDay(const QModelIndex& indexTop, const QModelIndex& indexBottom);

        // Проверка индексов ячеек таблиц
        bool isValidIndexes(const QModelIndex& topIndex, const QModelIndex& bottomIndex);

        // Функция выделяет в таблице расписания доступные для установки ячейки
        void selectSchedulerCell(const QModelIndex& index);

    private:
        Ui::MainWindow *ui;

        LineDaysTable lineDaysTable;
        SchedulerTable schedulerTable;
        ScoreInfoBoard scoreInfoBoard;

        // Флаг говорит являлся ли последний клик по ячейкам
        // в таблице расписания. false -- если нет или не было
        // ниразу
        bool isLastSchedulerTableClickedCell = false;

        // Виджет для показа подсказок работы с таблицами
        QLabel helpManagerLabel;

    protected:
        void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
