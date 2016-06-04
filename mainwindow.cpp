#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "celldelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->tableView->setModel(&model);
    ui->tableView->setItemDelegate(new CellDelegate());

//    model.view = ui->tableView;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::load(const QString& fileName) {
    model.load(fileName);
    ui->tableView->resizeColumnsToContents();

//    static const int size = 200;
//    for (int i = 0; i < ui->tableView->colorCount(); i++) {
//        ui->tableView->horizontalHeader()->resizeSection(i, size);
//    }
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}
