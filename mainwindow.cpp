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

    // TODO: привести connect к одному виду
    connect(&lineDaysTable, &QTableView::clicked, this, &selectSchedulerCell);

    // TODO: привести в нормальный вид, пока все запутанно и похоже на костыль
    lineDaysTable.model.busmanTableModel = &schedulerTable.model;

    // Прячем горизонтальный ползунок
    lineDaysTable.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Соединяем горинонтальный ползунок таблицы с расписанием с горизонтальным ползунком таблицы линий
    connect(schedulerTable.horizontalScrollBar(), &QAbstractSlider::valueChanged,
            lineDaysTable.horizontalScrollBar(), &QAbstractSlider::setValue);


//    // При кликах на ячейках проверяем состояния виджетов, например кнопок -- делаем активные
//    // или напротив деактивируем
//    connect(&lineDaysTable, &QTableView::clicked, this, &updateStates);
//    connect(&schedulerTable, &QTableView::clicked, this, &updateStates);

    // TODO: лучше перенести в поля класса
    auto clickedCellTables = [=](const QModelIndex& index) {
        auto topIndex = lineDaysTable.currentIndex();
        auto bottomIndex = schedulerTable.currentIndex();

        // Проверка валидности индексов
        bool enabled = isValidIndexes(topIndex, bottomIndex);
        qDebug() << "clickedCellTables" << index << sender() << isLastSchedulerTableClickedCell << enabled;

        isLastSchedulerTableClickedCell = sender() == &schedulerTable;
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
    };

    // При кликах на ячейках проверяем состояния виджетов, например кнопок -- делаем активные
    // или напротив деактивируем
    connect(&lineDaysTable, &QTableView::clicked, this, clickedCellTables);
    connect(&schedulerTable, &QTableView::clicked, this, clickedCellTables);

    auto tableLayout = new QVBoxLayout();
    tableLayout->setMargin(0);
    tableLayout->addWidget(&lineDaysTable);
    tableLayout->addWidget(&schedulerTable);

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
        schedulerTable.model.isVisibleCellText = visible;

        // Говорим представлению обновиться
        schedulerTable.model.sayViewUpdate();
    });

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);

    updateStates();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::load(const QString& fileName) {
    schedulerTable.model.load(fileName);

    static const int size = 60;
    for (int i = 0; i < schedulerTable.model.columnCount(); i++) {
        schedulerTable.horizontalHeader()->resizeSection(i, size);
    }
    schedulerTable.horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    schedulerTable.verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    lineDaysTable.model.reset__();

    // Устанавливаем размер столбцов как в tableView
    for (int i = 0; i < lineDaysTable.model.columnCount(); i++) {
        auto size = schedulerTable.horizontalHeader()->sectionSize(i);
        lineDaysTable.horizontalHeader()->resizeSection(i, size);
    }

    static const int width_vertical_header = 90;
    schedulerTable.verticalHeader()->setFixedWidth(width_vertical_header);
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

    schedulerTable.model.saveAs(fileName);
}

void MainWindow::read_settings() {
    QSettings config("config",  QSettings::IniFormat);
    restoreState(config.value("MainWindow_State").toByteArray());
    restoreGeometry(config.value("MainWindow_Geometry").toByteArray());
}

void MainWindow::write_settings() {
    QSettings config("config",  QSettings::IniFormat);
    config.setValue("MainWindow_State", saveState());
    config.setValue("MainWindow_Geometry", saveGeometry());
}

void MainWindow::updateStates() {
    ui->actionSelectSun->setEnabled(false);
    ui->actionSelectMoon->setEnabled(false);

    // TODO: добавление проверки isValidSetDay

    auto topIndex = lineDaysTable.currentIndex();
    auto bottomIndex = schedulerTable.currentIndex();

    // Проверка валидности индексов
    bool enabled = isValidIndexes(topIndex, bottomIndex);
    if (enabled) {
        // Получаем пару значений выбранной ячейки таблицы сверху
        auto days = lineDaysTable.model.get(topIndex);

        // Проверяем выбранные стороны и что в них есть значения
        bool enabledLeft = lineDaysTable.delegate.selectedSide == Side::Left && days.first != DayKind::NONE;
        bool enabledRight = lineDaysTable.delegate.selectedSide == Side::Right && days.second != DayKind::NONE;

        // TODO: lineDaysTable.delegate заменить на обращение к методу lineDaysTable
        ui->actionSelectSun->setEnabled(enabledLeft);
        ui->actionSelectMoon->setEnabled(enabledRight);
    }
}

void MainWindow::closeEvent(QCloseEvent*) {
    write_settings();
    qApp->quit();
}
