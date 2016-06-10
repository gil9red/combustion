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

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::MainWindow *ui;

        LineDaysTable lineDaysTable;
        QTableView tableView;
        BusmanTableModel model;


};

#endif // MAINWINDOW_H
