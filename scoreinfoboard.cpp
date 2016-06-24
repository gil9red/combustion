#include "scoreinfoboard.h"
#include "ui_scoreinfoboard.h"
#include <QDebug>


QString getFormatFloatValue(float value) {
    return QString::number(value,  'f',  1);
}


ScoreInfoBoard::ScoreInfoBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScoreInfoBoard) {

    ui->setupUi(this);

    enumValueNumbersMap[ShiftPreferences]          = 0;
    enumValueNumbersMap[DayoffPreferences]         = 0;
    enumValueNumbersMap[UnassignedShifts]          = 0;
    enumValueNumbersMap[LongRests]                 = 0;
    enumValueNumbersMap[EarlyAfterLateShifts]      = 0;
    enumValueNumbersMap[ConsecutiveLateShifts]     = 0;
    enumValueNumbersMap[DeviationTargetLateShifts] = 0;

    enumValueLabelValuesMap[ShiftPreferences]          = ui->label_ShiftPreferences_Value;
    enumValueLabelValuesMap[DayoffPreferences]         = ui->label_DayoffPreferences_Value;
    enumValueLabelValuesMap[UnassignedShifts]          = ui->label_UnassignedShifts_Value;
    enumValueLabelValuesMap[LongRests]                 = ui->label_LongRests_Value;
    enumValueLabelValuesMap[EarlyAfterLateShifts]      = ui->label_EarlyAfterLateShifts_Value;
    enumValueLabelValuesMap[ConsecutiveLateShifts]     = ui->label_ConsecutiveLateShifts_Value;
    enumValueLabelValuesMap[DeviationTargetLateShifts] = ui->label_DeviationTargetLateShifts_Value;

    enumValueLabelNumbersMap[ShiftPreferences]          = ui->label_ShiftPreferences_Number;
    enumValueLabelNumbersMap[DayoffPreferences]         = ui->label_DayoffPreferences_Number;
    enumValueLabelNumbersMap[UnassignedShifts]          = ui->label_UnassignedShifts_Number;
    enumValueLabelNumbersMap[LongRests]                 = ui->label_LongRests_Number;
    enumValueLabelNumbersMap[EarlyAfterLateShifts]      = ui->label_EarlyAfterLateShifts_Number;
    enumValueLabelNumbersMap[ConsecutiveLateShifts]     = ui->label_ConsecutiveLateShifts_Number;
    enumValueLabelNumbersMap[DeviationTargetLateShifts] = ui->label_DeviationTargetLateShifts_Number;

    enumValueLabelResultsMap[ShiftPreferences]          = ui->label_ShiftPreferences_Result;
    enumValueLabelResultsMap[DayoffPreferences]         = ui->label_DayoffPreferences_Result;
    enumValueLabelResultsMap[UnassignedShifts]          = ui->label_UnassignedShifts_Result;
    enumValueLabelResultsMap[LongRests]                 = ui->label_LongRests_Result;
    enumValueLabelResultsMap[EarlyAfterLateShifts]      = ui->label_EarlyAfterLateShifts_Result;
    enumValueLabelResultsMap[ConsecutiveLateShifts]     = ui->label_ConsecutiveLateShifts_Result;
    enumValueLabelResultsMap[DeviationTargetLateShifts] = ui->label_DeviationTargetLateShifts_Result;

    refresh();
}

ScoreInfoBoard::~ScoreInfoBoard() {
    delete ui;
}

void ScoreInfoBoard::scheduleAnalysis() {
    // Сброс очков
    for (auto key: enumValueNumbersMap.keys()) {
        enumValueNumbersMap[key] = 0;
    }

    if (schedulerTableModel != nullptr) {
        // У каждого водителя maxGoodDayLateNumber ночной смены, но не должно превышать maxGoodDayLateNumber
        // <кол-во водителей> * maxGoodDayLateNumber
        enumValueNumbersMap[DeviationTargetLateShifts] = schedulerTableModel->rowCount() * maxGoodDayLateNumber;

        for (int row = 0; row < schedulerTableModel->rowCount(); row++) {
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                auto index = schedulerTableModel->index(row, column);
                auto textCell = schedulerTableModel->data(index, SchedulerTableModel::WishDayRole).toString();

                // TODO:
                if (textCell == "RR") {
                    enumValueNumbersMap[DayoffPreferences]++;
                }
            }

            // Количество DayoffPreferences у водителя
            int numberShiftPreferences = 0;
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                // TODO: switch
                auto index = schedulerTableModel->index(row, column);
                auto day = schedulerTableModel->getDayKind(index);
                auto textCell = schedulerTableModel->data(index, SchedulerTableModel::WishDayRole).toString();

                if (textCell == "RR") {
                    if (day == DayKind::LINE_1_NIGHT
                            || day == DayKind::LINE_2_NIGHT
                            || day == DayKind::LINE_3_NIGHT
                            || day == DayKind::LINE_1_DAY
                            || day == DayKind::LINE_2_DAY
                            || day == DayKind::LINE_3_DAY) {
                        numberShiftPreferences++;
                    }
                }
            }

            // TODO:
            if (numberShiftPreferences > 0) {
                enumValueNumbersMap[DayoffPreferences] -= numberShiftPreferences;
            } else {
                enumValueNumbersMap[DayoffPreferences] += numberShiftPreferences;
            }

            // Количество ShiftPreferences у водителя ночью
            int numberShiftPreferencesNight = 0;
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                // TODO: switch
                auto index = schedulerTableModel->index(row, column);
                auto day = schedulerTableModel->getDayKind(index);
                auto textCell = schedulerTableModel->data(index, SchedulerTableModel::WishDayRole).toString();

                if (textCell == "NN") {
                    if (day == DayKind::LINE_1_NIGHT
                            || day == DayKind::LINE_2_NIGHT
                            || day == DayKind::LINE_3_NIGHT) {
                        numberShiftPreferencesNight++;
                    }
                }
            }
            // TODO:
            if (numberShiftPreferencesNight > 0) {
                enumValueNumbersMap[ShiftPreferences] += numberShiftPreferencesNight;
            } else {
                enumValueNumbersMap[ShiftPreferences] -= numberShiftPreferencesNight;
            }

            // Количество DayoffPreferences у водителя день
            int numberShiftPreferencesDay = 0;
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                // TODO: switch
                auto index = schedulerTableModel->index(row, column);
                auto day = schedulerTableModel->getDayKind(index);
                auto textCell = schedulerTableModel->data(index, SchedulerTableModel::WishDayRole).toString();

                if (textCell == "DD") {
                    if (day == DayKind::LINE_1_DAY
                            || day == DayKind::LINE_2_DAY
                            || day == DayKind::LINE_3_DAY) {
                        numberShiftPreferencesDay++;
                    }
                }
            }
            // TODO:
            if (numberShiftPreferencesDay > 0) {
                enumValueNumbersMap[ShiftPreferences] += numberShiftPreferencesDay;
            } else {
                enumValueNumbersMap[ShiftPreferences] -= numberShiftPreferencesDay;
            }

            // Количество ночных смен у водителя
            int numberLateDay = 0;
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                // TODO: switch
                auto index = schedulerTableModel->index(row, column);
                auto day = schedulerTableModel->getDayKind(index);
                if (day == DayKind::LINE_1_NIGHT
                        || day == DayKind::LINE_2_NIGHT
                        || day == DayKind::LINE_3_NIGHT) {
                    numberLateDay++;
                }
            }

            if (numberLateDay > 0) {
                // TODO:
                if (numberLateDay > maxGoodDayLateNumber) {
                    enumValueNumbersMap[DeviationTargetLateShifts] -= maxGoodDayLateNumber;
                    enumValueNumbersMap[DeviationTargetLateShifts] += numberLateDay - maxGoodDayLateNumber;
                } else {
                    enumValueNumbersMap[DeviationTargetLateShifts] -= numberLateDay;
                }
            }
        }
    }

    //    if (lineDaysTable != nullptr){
    //        for (int row = 0; row < lineDaysTable->model.rowCount(); row++){
    //            for (int column = 0; column < lineDaysTable->model.columnCount(); column++){
    //                auto index =  lineDaysTable->model.index(row,column);
    //                // TODO:
    //                // Вывести общее количество Day и Night
    //                //                auto itemRole = lineDaysTable->model.data(index,);

    //                //                enumValueLabelNumbersMap[UnassignedShifts]
    //            }
    //        }
    //    }
}

void ScoreInfoBoard::refresh() {
    scheduleAnalysis();

    // Подсчет очков
    float score = 0.0;

    // Заполнение a x b = c
    for (auto key: enumValueNumbersMap.keys()) {
        // Заполнение a
        auto label = enumValueLabelValuesMap[key];
        label->setText(getFormatFloatValue(key));

        // Заполнение b
        auto number = enumValueNumbersMap[key];
        label = enumValueLabelNumbersMap[key];
        label->setText(getFormatFloatValue(number));

        float value = key * number;
        score += value;

        // Заполнение c
        label = enumValueLabelResultsMap[key];
        label->setText(getFormatFloatValue(value));
    }

    ui->label_Score->setText(getFormatFloatValue(score));
}
