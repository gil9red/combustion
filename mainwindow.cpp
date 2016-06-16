#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scheduler_celldelegate.h"
#include <QFileDialog>
#include <QDebug>
#include <QScrollBar>
#include <QSettings>
#include "verticalschedulerheaderview.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    read_settings();

    tableView.setModel(&model);
    tableView.setItemDelegate(new SchedulerCellDelegate());

    // Заголовок будет в lineDaysTable
    tableView.horizontalHeader()->hide();
    tableView.setVerticalHeader(new VerticalSchedulerHeaderView());

    tableView.setSelectionBehavior(QAbstractItemView::SelectItems);
    tableView.setSelectionMode(QAbstractItemView::SingleSelection);

//    // TODO: пример выборочного выделение ячеек
//    connect(&lineDaysTable, &QTableView::clicked, this, [=](const QModelIndex& index) {
//        // Очищение выделения
//        tableView.selectionModel()->clearSelection();

//        int column = index.column();

//        for (int row = 0; row < model.rowCount(); row++) {
//            Busman* busman = model.get(row);
//            if (busman == nullptr) {
//                qWarning() << "connect(&lineDaysTable, &QTableView::clicked... busman is nullptr, row:" << row;
//                continue;
//            }

//            // TODO: через модель сделать получение
//            if (column >= busman->wishesOnSchedule.length()) {
//                qWarning() << "column >= busman->wishesOnSchedule.length()... column: " << column << ", length: " << busman->wishesOnSchedule.length();
//                continue;
//            }
//            auto text = busman->wishesOnSchedule[column];

//            // Выделяются только ячейки с RR
//            if (text != "RR") {
//                continue;
//            }

//            auto indexView = model.index(row, column);

//            // Выделение ячейки
//            tableView.selectionModel()->select(indexView, QItemSelectionModel::Select);
//        }
//    });

    // TODO: привести в нормальный вид, пока все запутанно и похоже на костыль
    lineDaysTable.model.busmanTableModel = &model;

    // Прячем горизонтальный ползунок
    lineDaysTable.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Соединяем горинонтальный ползунок таблицы с расписанием с горизонтальным ползунком таблицы линий
    connect(tableView.horizontalScrollBar(), &QAbstractSlider::valueChanged,
            lineDaysTable.horizontalScrollBar(), &QAbstractSlider::setValue);

    auto tableLayout = new QVBoxLayout();
    tableLayout->setMargin(0);
    tableLayout->addWidget(&lineDaysTable);
    tableLayout->addWidget(&tableView);

    auto tablesView = new QWidget();
    tablesView->setObjectName("TablesView");
    tablesView->setStyleSheet("QWidget#TablesView { background-color: gray; }");
    tablesView->setLayout(tableLayout);

    auto mainLayout = new QHBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->addWidget(&scoreInfoBoard);
    mainLayout->addWidget(tablesView);

    auto centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);

    connect(ui->actionShowCellText, &QAction::triggered, [=](bool visible) {
        model.isVisibleCellText = visible;

        // Говорим представлению обновиться
        model.sayViewUpdate();
    });

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::load(const QString& fileName) {
    model.load(fileName);

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

    static const int width_vertical_header = 90;
    tableView.verticalHeader()->setFixedWidth(width_vertical_header);
    lineDaysTable.verticalHeader()->setFixedWidth(width_vertical_header);

    for (int i = 0; i < lineDaysTable.model.rowCount(); i++) {
        lineDaysTable.verticalHeader()->resizeSection(i, size / 2);
    }

    lineDaysTable.horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    lineDaysTable.verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // TODO:
    auto heightLineDays = lineDaysTable.horizontalHeader()->height() + 5;
    for (auto i = 0; i < lineDaysTable.verticalHeader()->count(); i++) {
        heightLineDays += lineDaysTable.verticalHeader()->sectionSize(i);
    }
    lineDaysTable.setFixedHeight(heightLineDays);
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

void MainWindow::read_settings(){
    QSettings config("config",  QSettings::IniFormat);
    restoreState(config.value("MainWindow_State").toByteArray());
    restoreGeometry(config.value("MainWindow_Geometry").toByteArray());
}

void MainWindow::write_settings(){

    QSettings config("config",  QSettings::IniFormat);
    config.setValue("MainWindow_State", saveState());
    config.setValue("MainWindow_Geometry", saveGeometry());
}

void MainWindow::closeEvent(QCloseEvent*){
       write_settings();
       qApp->quit();
}
