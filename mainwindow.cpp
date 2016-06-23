#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scheduler_celldelegate.h"
#include <QFileDialog>
#include <QDebug>
#include <QScrollBar>
#include <QSettings>
#include "verticalschedulerheaderview.h"
#include <QDockWidget>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // TODO: привести connect к одному виду
    connect(&lineDaysTable, &QTableView::clicked, this, &selectSchedulerCell);

    // TODO: привести в нормальный вид, пока все запутанно и похоже на костыль
    lineDaysTable.model.busmanTableModel = &schedulerTable.model;

    // Прячем горизонтальный ползунок
    lineDaysTable.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Соединяем горинонтальный ползунок таблицы с расписанием с горизонтальным ползунком таблицы линий
    connect(schedulerTable.horizontalScrollBar(), &QAbstractSlider::valueChanged,
            lineDaysTable.horizontalScrollBar(), &QAbstractSlider::setValue);

    lineDaysTable.viewport()->installEventFilter(this);
    schedulerTable.viewport()->installEventFilter(this);

    // TODO: перевод
    scoreInfoBoard.busmanTableModel = &schedulerTable.model;
    auto scoreInfoBoardDockWidget = new QDockWidget("Score Info");
    scoreInfoBoardDockWidget->setObjectName("ScoreInfoBoardDockWidget");
    scoreInfoBoardDockWidget->setWidget(&scoreInfoBoard);
    addDockWidget(Qt::LeftDockWidgetArea, scoreInfoBoardDockWidget);

    // TODO: перевод
    helpManagerLabel.setWordWrap(true);
    helpManagerLabel.setAlignment(Qt::AlignTop | Qt::AlignLeft);
    auto helpManagerLabelDockWidget = new QDockWidget("helpManagerLabel");
    helpManagerLabelDockWidget->setObjectName("HelpManagerLabelDockWidget");
    helpManagerLabelDockWidget->setWidget(new QWidget());
    helpManagerLabelDockWidget->widget()->setLayout(new QVBoxLayout());
    helpManagerLabelDockWidget->widget()->layout()->addWidget(&helpManagerLabel);
    addDockWidget(Qt::LeftDockWidgetArea, helpManagerLabelDockWidget);

    auto mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->addWidget(&lineDaysTable);
    mainLayout->addWidget(&schedulerTable);

    auto tablesView = new QWidget();
    tablesView->setObjectName("TablesView");
    tablesView->setStyleSheet("QWidget#TablesView { background-color: gray; }");
    tablesView->setLayout(mainLayout);

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

    // TODO: реализовать. Это menuBar'ы хранящие в себе действия для изменения
    // видимости toolBar'ов и dockWidget'ов
//    # Все действия к прикрепляемым окнам поместим в меню
//    for dock in self.findChildren(QDockWidget):
//        self.ui.menuDockWindow.addAction(dock.toggleViewAction())

//    # Все действия к toolbar'ам окнам поместим в меню
//    for tool in self.findChildren(QToolBar):
//        self.ui.menuTools.addAction(tool.toggleViewAction())

    updateStates();
    read_settings();
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

    updateStates();
}

void MainWindow::open() {
    auto fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty()) {
        return;
    }

    load(fileName);
}

void MainWindow::saveAs() {
    auto fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty()) {
        return;
    }

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
    auto topIndex = lineDaysTable.currentIndex();
    auto bottomIndex = schedulerTable.currentIndex();

    // TODO:
    QString helpText;
    helpText = "◑ ◔ ╔═╗ ║▓▒░░░░░░░░░░░░░░░░░░ ╚═╝ IMMA CHARGIN MAH LAZER!";

    if (!topIndex.isValid() && !bottomIndex.isValid()) {
        helpText = "Для назначения кликни на ячейку таблицы выше.";
    } else {
        if (topIndex.isValid() && !bottomIndex.isValid()) {
            helpText = "Хорошо, в таблице ниже выбери ячейку для вставки значения.";
        }
    }
    helpText += "\n\nДля возврата значения кликни правой кнопкой мышки.";
    helpManagerLabel.setText(helpText);

    // Обновляем виджет с очками
    scoreInfoBoard.refresh();

    // TODO: неиспользуется
//    ui->actionSelectSun->setEnabled(false);
//    ui->actionSelectMoon->setEnabled(false);

//    // TODO: добавление проверки isValidSetDay

//    auto topIndex = lineDaysTable.currentIndex();
//    auto bottomIndex = schedulerTable.currentIndex();

//    // Проверка валидности индексов
//    bool enabled = isValidIndexes(topIndex, bottomIndex);
//    if (enabled) {
//        // Получаем пару значений выбранной ячейки таблицы сверху
//        auto days = lineDaysTable.model.get(topIndex);

//        // Проверяем выбранные стороны и что в них есть значения
//        bool enabledLeft = lineDaysTable.delegate.selectedSide == Side::Left && days.first != DayKind::NONE;
//        bool enabledRight = lineDaysTable.delegate.selectedSide == Side::Right && days.second != DayKind::NONE;

//        // TODO: lineDaysTable.delegate заменить на обращение к методу lineDaysTable
//        ui->actionSelectSun->setEnabled(enabledLeft);
//        ui->actionSelectMoon->setEnabled(enabledRight);
//    }
}

void MainWindow::closeEvent(QCloseEvent*) {
    write_settings();
    qApp->quit();
}
