#ifndef SCOREINFOBOARD_H
#define SCOREINFOBOARD_H

#include <QWidget>

namespace Ui {
    class ScoreInfoBoard;
}

#include <QMap>
#include <QLabel>
#include "scheduler_table_model.h"
#include "linedaystable.h"


class ScoreInfoBoard : public QWidget {
        Q_OBJECT

    public:
        explicit ScoreInfoBoard(QWidget *parent = 0);
        ~ScoreInfoBoard();

        void refresh();

        // Функция подсчета количества очков
        float calcScore();

    public:
        enum EnumValue {
            ShiftPreferences          = 3,
            DayoffPreferences         = 4,
            UnassignedShifts          = -20,
            LongRests                 = 5,
            EarlyAfterLateShifts      = -30,
            ConsecutiveLateShifts     = -10,
            DeviationTargetLateShifts = -8,
        };

        struct Data {
            // TODO: добавить операторы ++/-/+, которые работают с number

            // Количество случаев
            int number = 0;

            // Отображение значения случая метрики
            QLabel* labelValue = nullptr;

            // Отображение количества случаев метрики
            QLabel* labelNumber = nullptr;

            // Отображение результата подсчета случаев метрики
            QLabel* labelResult = nullptr;

            Data(int number, QLabel* labelValue, QLabel* labelNumber, QLabel* labelResult) {
                this->number = number;
                this->labelValue = labelValue;
                this->labelNumber = labelNumber;
                this->labelResult = labelResult;
            }
        };

        // Максимальное количество разрешенных у водителей ночных смен.
        const static int maxGoodDayLateNumber = 4;

        SchedulerTableModel* schedulerTableModel = nullptr;
        LineDaysTable* lineDaysTable = nullptr;

    private:
        // Функция подсчитывает случаи метрики
        void scheduleAnalysis();

    private:
        Ui::ScoreInfoBoard *ui;
        QMap<EnumValue, Data*> enumValueDataMap;
};

#endif // SCOREINFOBOARD_H
