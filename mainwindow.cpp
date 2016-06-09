#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "celldelegate.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->tableView->setModel(&model);
    ui->tableView->setItemDelegate(new CellDelegate());

//    connect(ui->actionShowCellText, SIGNAL(triggered(bool)), SLOT(setVisibleCellText(bool)));
//
//    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(open()));
//    connect(ui->actionSaveAs, SIGNAL(triggered()), SLOT(saveAs()));

//    connect(ui->actionShowCellText, &QAction::triggered, this, &MainWindow::setVisibleCellText);
    connect(ui->actionShowCellText, &QAction::triggered, [=](bool visible) {
        model.isVisibleCellText = visible;

        // Говорим представлению обновиться
        model.sayViewUpdate();
    });

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);

    ui->dayLinesTableWidget->hide();
//    for (auto key: model.lineDaysIconsMap.keys()) {
//        QImage icon = model.lineDaysIconsMap[key];
//        QListWidgetItem* item = new QListWidgetItem(QIcon(QPixmap::fromImage(icon)), "");
//        ui->listWidget->addItem(item);
//    }
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

void MainWindow::open() {
    const QString& fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty())
        return;

    load(fileName);
}

void MainWindow::saveAs() {
    const QString& fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return;

    model.saveAs(fileName);
}

//void MainWindow::setVisibleCellText(bool visible) {
//    model.isVisibleCellText = visible;

//    // Говорим представлению обновиться
//    model.sayViewUpdate();
//}
