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

    // TODO: привести в нормальный вид, пока все запутанно и похоже на костыль
    lineDaysTable.model.schedulerTableModel = &schedulerTable.model;

    // Прячем горизонтальный ползунок
    lineDaysTable.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    lineDaysTable.viewport()->installEventFilter(this);
    schedulerTable.viewport()->installEventFilter(this);

    // TODO: перевод
    scoreInfoBoard.schedulerTableModel = &schedulerTable.model;
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

    connect(&lineDaysTable, &QTableView::clicked, this, selectSchedulerCell);

    // Соединяем горинонтальный ползунок таблицы с расписанием с горизонтальным ползунком таблицы линий
    connect(schedulerTable.horizontalScrollBar(), QAbstractSlider::valueChanged,
            lineDaysTable.horizontalScrollBar(), QAbstractSlider::setValue);

    connect(ui->actionShowCellText, QAction::triggered, [=](bool visible) {
        schedulerTable.model.isVisibleCellText = visible;

        // Говорим представлению обновиться
        schedulerTable.model.sayViewUpdate();
    });

    connect(ui->actionOpen, QAction::triggered, this, open);
    connect(ui->actionSaveAs, QAction::triggered, this, saveAs);

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

    // Ширина колонок
    static const int width_columns = 60;
    static const int width_vertical_header = 90;

    auto schedulerHHeader = schedulerTable.horizontalHeader();
    auto schedulerVHeader = schedulerTable.verticalHeader();

    for (int i = 0; i < schedulerTable.model.columnCount(); i++) {
        schedulerHHeader->resizeSection(i, width_columns);
    }
    schedulerHHeader->setSectionResizeMode(QHeaderView::Fixed);
    schedulerVHeader->setSectionResizeMode(QHeaderView::Fixed);

    lineDaysTable.model.reset__();

    auto lineDaysHHeader = lineDaysTable.horizontalHeader();
    auto lineDaysVHeader = lineDaysTable.verticalHeader();

    // Устанавливаем размер столбцов как в tableView
    for (int i = 0; i < lineDaysTable.model.columnCount(); i++) {
        auto size = schedulerHHeader->sectionSize(i);
        lineDaysHHeader->resizeSection(i, size);
    }

    schedulerVHeader->setFixedWidth(width_vertical_header);
    lineDaysVHeader->setFixedWidth(width_vertical_header);

    for (int i = 0; i < lineDaysTable.model.rowCount(); i++) {
        lineDaysVHeader->resizeSection(i, width_columns / 2);
    }

    lineDaysHHeader->setSectionResizeMode(QHeaderView::Fixed);
    lineDaysVHeader->setSectionResizeMode(QHeaderView::Fixed);

    auto heightLineDays = lineDaysHHeader->height() + 5;
    for (auto i = 0; i < lineDaysVHeader->count(); i++) {
        heightLineDays += lineDaysVHeader->sectionSize(i);
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
    helpText = "◑ ◔\n";
    helpText += "╔═╗\n";
    helpText += "║▓▒░░░░░░░░░░░░░░░░░░\n";
    helpText += "╚═╝\n";
    helpText += "IMMA CHARGIN MAH LAZER!\n\n";

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

bool MainWindow::isValidSetDay(const QModelIndex& indexTop, const QModelIndex& indexBottom) {
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
bool MainWindow::isValidIndexes(const QModelIndex& topIndex, const QModelIndex& bottomIndex) {
    // Проверка валидности индексов
    bool isValid = topIndex.isValid() && bottomIndex.isValid();

    // Проверка того, что столбцы одинаковые
    isValid = isValid && topIndex.column() == bottomIndex.column();
    return isValid;
}

// Функция выделяет в таблице расписания доступные для установки ячейки
void MainWindow::selectSchedulerCell(const QModelIndex& index) {
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
bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
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

void MainWindow::closeEvent(QCloseEvent*) {
    write_settings();
    qApp->quit();
}
