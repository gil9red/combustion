#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "celldelegate.h"
#include <QFileDialog>
#include <QDebug>
#include <QScrollBar>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

//    ui->tableView->setModel(&model);
//    ui->tableView->setItemDelegate(new CellDelegate());

    tableView.setModel(&model);
    tableView.setItemDelegate(new CellDelegate());

    // TODO: привести в нормальный вид, пока все запутанно и похоже на костыль
    lineDaysTable.model.busmanTableModel = &model;

    // Прячем горизонтальный ползунок
    lineDaysTable.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Соединяем горинонтальный ползунок таблицы с расписанием с горизонтальным ползунком таблицы линий
    connect(tableView.horizontalScrollBar(), &QAbstractSlider::valueChanged,
            lineDaysTable.horizontalScrollBar(), &QAbstractSlider::setValue);

    auto mainLayout = new QVBoxLayout();
    mainLayout->addWidget(&lineDaysTable);
    mainLayout->addWidget(&tableView);

    auto centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);

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

//    ui->dayLinesTableWidget->hide();
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
//    ui->tableView->resizeColumnsToContents();
//    tableView.resizeColumnsToContents();
    static const int size = 60;
    for (int i = 0; i < tableView.model()->columnCount(); i++) {
        tableView.horizontalHeader()->resizeSection(i, size);
    }
    tableView.horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableView.verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // Устанавливаем размер стоблцов как в tableView
    // TODO: привести к одной ширине столбцов с учетом разного количества столбцов
    // TODO:
    lineDaysTable.model.reset__();

    for (int i = 0; i < lineDaysTable.model.columnCount(); i++) {
        auto size = tableView.horizontalHeader()->sectionSize(i);
        lineDaysTable.horizontalHeader()->resizeSection(i, size);
    }

    tableView.verticalHeader()->setFixedWidth(size);
    lineDaysTable.verticalHeader()->setFixedWidth(size);

    for (int i = 0; i < lineDaysTable.model.rowCount(); i++) {
        lineDaysTable.verticalHeader()->resizeSection(i, size / 2);
    }

//    lineDaysTable.verticalHeader()->setMinimumSectionSize(size * 3);
//    lineDaysTable.verticalHeader()->setMaximumSectionSize(size * 3);

//    lineDaysTable.verticalHeader()->setFixedWidth(tableView.verticalHeader()->width());
//    lineDaysTable.verticalHeader()->setFixedHeight(lineDaysTable.verticalHeader()->width() / 2);

    lineDaysTable.horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    lineDaysTable.verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // TODO:
    auto heightLineDays = lineDaysTable.horizontalHeader()->height() + 5;
    for (auto i = 0; i < lineDaysTable.verticalHeader()->count(); i++) {
        heightLineDays += lineDaysTable.verticalHeader()->sectionSize(i);
    }
    lineDaysTable.setFixedHeight(heightLineDays);

//    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
//    lineDaysTable.model.sayViewUpdate();

//    lineDaysTable.resizeColumnsToContents();

//    static const int size = 200;
//    for (int i = 0; i < ui->tableView->colorCount(); i++) {
//        ui->tableView->horizontalHeader()->resizeSection(i, size);
//    }
//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

void MainWindow::open() {
    auto fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty())
        return;

    load(fileName);
}

void MainWindow::saveAs() {
    auto fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return;

    model.saveAs(fileName);
}

//void MainWindow::setVisibleCellText(bool visible) {
//    model.isVisibleCellText = visible;

//    // Говорим представлению обновиться
//    model.sayViewUpdate();
//}
