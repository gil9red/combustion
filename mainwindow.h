#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "busmantablemodel.h"
#include "linedaystable.h"
#include <QResizeEvent>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
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

            auto day = lineDaysTable.model.getLeft(topIndex);
            qDebug() << "sun" << day;
            model.setDayKind(bottomIndex, day);

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

            auto day = lineDaysTable.model.getRight(topIndex);
            qDebug() << "moon" << day;
            model.setDayKind(bottomIndex, day);

            model.sayViewUpdate();
            lineDaysTable.model.sayViewUpdate();
        }

    private:
        Ui::MainWindow *ui;

        LineDaysTable lineDaysTable;

        // TODO: в отдельный виджет как LineDaysTable
        QTableView tableView;
        BusmanTableModel model;

    protected:
        void closeEvent(QCloseEvent*);

};

#endif // MAINWINDOW_H
