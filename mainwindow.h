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
            // Проверяем, что проверка по индекса проходит
            if (!isValidIndexes(indexTop, indexBottom)) {
                return false;
            }

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

        // Служит продвинутым аналогом сигнала clicked, который раньше отлавливался от таблиц.
        bool eventFilter(QObject* obj, QEvent* event) {
            if (obj != lineDaysTable.viewport() && obj != schedulerTable.viewport()) {
                return false;
            }

            if (event->type() == QEvent::MouseButtonRelease) {
                auto mouseEvent = static_cast <QMouseEvent*> (event);
                auto pos = mouseEvent->pos();

                // Проверяем, что клик произошел в какую-нибудь ячейку
                if (!lineDaysTable.indexAt(pos).isValid() && !schedulerTable.indexAt(pos).isValid()) {
                    return false;
                }

                // TODO: разобраться с дубликатами
                if (mouseEvent->button() == Qt::LeftButton) {
                    auto topIndex = lineDaysTable.currentIndex();
                    auto bottomIndex = schedulerTable.currentIndex();

                    // Проверка возможности вставки значения -- тоже самое используется
                    // для выделения ячеек таблицы расписания
                    bool enabled = isValidSetDay(topIndex, bottomIndex);

                    isLastSchedulerTableClickedCell = obj == schedulerTable.viewport();
                    if (isLastSchedulerTableClickedCell && enabled) {
                        // Получаем пару значений выбранной ячейки таблицы сверху
                        auto days = lineDaysTable.model.get(topIndex);

                        // Проверяем выбранные стороны и что в них есть значения
                        bool enabledLeft = lineDaysTable.delegate.selectedSide == Side::Left && days.first != DayKind::NONE;
                        bool enabledRight = lineDaysTable.delegate.selectedSide == Side::Right && days.second != DayKind::NONE;

                        if (enabledLeft) {
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
                            schedulerTable.model.setDayKind(bottomIndex, days.first);

                            // TODO: дубликат
                            // TODO: перерисовывать лучше только изменившуюся ячейку, а не всю таблицу
                            schedulerTable.model.sayViewUpdate();
                            lineDaysTable.model.sayViewUpdate();

                        } else if (enabledRight) {
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
                            schedulerTable.model.setDayKind(bottomIndex, days.second);

                            // TODO: дубликат
                            // TODO: перерисовывать лучше только изменившуюся ячейку, а не всю таблицу
                            schedulerTable.model.sayViewUpdate();
                            lineDaysTable.model.sayViewUpdate();
                        }
                    }

                    updateStates();

                    // Только правый клик по таблице расписания
                } else if (obj == schedulerTable.viewport() && mouseEvent->button() == Qt::RightButton) {
                    auto bottomIndex = schedulerTable.currentIndex();

                    auto day = schedulerTable.model.getDayKind(bottomIndex);
                    if (day == DayKind::NONE) {
                        return false;
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

                    updateStates();
                }

                return false;
            }

            return QMainWindow::eventFilter(obj, event);
        }

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
