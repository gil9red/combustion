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
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public:
        void load(const QString& fileName);

        void open();
        void saveAs();

        void read_settings();
        void write_settings();

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

            if (lineDaysTable.model.getLeft(topIndex) == Busman::DayKind::NONE
                    && lineDaysTable.model.getRight(topIndex) == Busman::DayKind::NONE) {
                qWarning() << "Cell is empty";
                return;
            }

            // Берем значение
            auto day = lineDaysTable.model.getLeft(topIndex);
            if (day == Busman::DayKind::NONE) {
                qWarning() << "Left value cell is empty";
                return;
            }

            // Убираем значение, которое забрали
            lineDaysTable.model.setLeft(topIndex, Busman::DayKind::NONE);

            auto bottomCellDay = model.getDayKind(bottomIndex);
            if (bottomCellDay != Busman::DayKind::NONE) {
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

            if (lineDaysTable.model.getLeft(topIndex) == Busman::DayKind::NONE
                    && lineDaysTable.model.getRight(topIndex) == Busman::DayKind::NONE) {
                qWarning() << "Cell is empty";
                return;
            }

            // Берем значение
            auto day = lineDaysTable.model.getRight(topIndex);
            if (day == Busman::DayKind::NONE) {
                qWarning() << "Right value cell is empty";
                return;
            }

            // Убираем значение, которое забрали
            lineDaysTable.model.setRight(topIndex, Busman::DayKind::NONE);

            auto bottomCellDay = model.getDayKind(bottomIndex);
            if (bottomCellDay != Busman::DayKind::NONE) {
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
            if (day == Busman::DayKind::NONE) {
                return;
            }

            // TODO: clear
            model.setDayKind(bottomIndex, Busman::DayKind::NONE);

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
