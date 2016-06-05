#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "busmantablemodel.h"

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

    void setVisibleCellText(bool visible);

private:
    Ui::MainWindow *ui;
    BusmanTableModel model;
};

#endif // MAINWINDOW_H
