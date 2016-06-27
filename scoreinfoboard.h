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
        void analysis();

        // Подсчет ConsecutiveLateShifts
        // TODO: cpp
        void analysisConsecutiveLateShifts(int row) {
            if (schedulerTableModel == nullptr) {
                return;
            }

            int consecutiveLateShiftsNumber = 0;
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                auto index = schedulerTableModel->index(row, column);
                auto day = schedulerTableModel->getDayKind(index);

                if (isNight(day)) {
                    consecutiveLateShiftsNumber++;
                    // Если 3 подряд найдено и текущая ячейка относится к тем
                    // лишним ночным сменам, выделяем красной рамкой
                    if (consecutiveLateShiftsNumber > 3 ) {
                        enumValueDataMap[ConsecutiveLateShifts]->number += (consecutiveLateShiftsNumber - consecutiveLateShiftsNumber);
                        enumValueDataMap[ConsecutiveLateShifts]->number++;
                    }

                // Последовательность прервана, обнуляем
                } else {
                    consecutiveLateShiftsNumber = 0;
                }
            }
        }

        // Подсчет DeviationTargetLateShifts
        // TODO: cpp
        void analysisDeviationTargetLateShifts(int numberLateDay) {
            // У каждого водителя maxGoodDayLateNumber ночной смены, но не должно превышать maxGoodDayLateNumber
            // <кол-во водителей> * maxGoodDayLateNumber
            enumValueDataMap[DeviationTargetLateShifts]->number = schedulerTableModel->rowCount() * maxGoodDayLateNumber;

            // TODO:
            if (numberLateDay > maxGoodDayLateNumber) {
                enumValueDataMap[DeviationTargetLateShifts]->number -= maxGoodDayLateNumber;
                enumValueDataMap[DeviationTargetLateShifts]->number += numberLateDay - maxGoodDayLateNumber;
            } else {
                enumValueDataMap[DeviationTargetLateShifts]->number -= numberLateDay;
            }
        }

        // Подсчет EarlyAfterLateShifts
        // TODO: cpp
        void analysisEarlyAfterLateShifts(int row) {
            //Проверим если водитель работал в ночную смену и поставили на утро
            //то водитель будет не доволен
            int earlyAfterLateShiftsDayNumber = 0;
            int earlyAfterLateShiftsNightNumber = 0;
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                auto index = schedulerTableModel->index(row, column);
                auto day = schedulerTableModel->getDayKind(index);

                if (isNight(day)) {
                    earlyAfterLateShiftsNightNumber++;
                } else {
                    earlyAfterLateShiftsNightNumber = 0;
                }

                if (isDay(day)) {
                    earlyAfterLateShiftsDayNumber++;
                    if (earlyAfterLateShiftsNightNumber < 2 && earlyAfterLateShiftsDayNumber < 2) {
                        enumValueDataMap[EarlyAfterLateShifts]->number++;
                    }
                } else {
                    earlyAfterLateShiftsDayNumber = 0;
                }
            }
        }

        // Подсчет LongRests
        // TODO: cpp
        void analysisLongRests(int row) {
            // TODO:
            //Проверка если есть три свободные ячеики и после третий занята сменой
            int longRestsNoneDayNumber = 0;
            int longRestsNoneNumber = 0;
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                auto index = schedulerTableModel->index(row, column);
                auto day = schedulerTableModel->getDayKind(index);
                switch (day){
                    case DayKind::NONE:
                        longRestsNoneNumber++;
                        break;
                    default:
                        break;
                }

                if (isNight(day)) {
                    longRestsNoneDayNumber++;
                    // Если 3 подряд найдено пустых значения и наидена смена
                    // TODO:
                    //Проходит только один раз и не повторяет больше
                    if (longRestsNoneNumber > 2 && longRestsNoneDayNumber < 2 ) {
                        enumValueDataMap[LongRests]->number++;
                    }
                }
            }
        }

        // После анализа нужно заполнить виджеты
        void fillForms();

    private:
        Ui::ScoreInfoBoard *ui;
        QMap<EnumValue, Data*> enumValueDataMap;
};

#endif // SCOREINFOBOARD_H
