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

    enumValueNumbersMap[EnumValue::ShiftPreferences]          = 0;
    enumValueNumbersMap[EnumValue::DayoffPreferences]         = 0;
    enumValueNumbersMap[EnumValue::UnassignedShifts]          = 0;
    enumValueNumbersMap[EnumValue::LongRests]                 = 0;
    enumValueNumbersMap[EnumValue::EarlyAfterLateShifts]      = 0;
    enumValueNumbersMap[EnumValue::ConsecutiveLateShifts]     = 0;
    enumValueNumbersMap[EnumValue::DeviationTargetLateShifts] = 0;

    enumValueLabelValuesMap[EnumValue::ShiftPreferences]          = ui->label_ShiftPreferences_Value;
    enumValueLabelValuesMap[EnumValue::DayoffPreferences]         = ui->label_DayoffPreferences_Value;
    enumValueLabelValuesMap[EnumValue::UnassignedShifts]          = ui->label_UnassignedShifts_Value;
    enumValueLabelValuesMap[EnumValue::LongRests]                 = ui->label_LongRests_Value;
    enumValueLabelValuesMap[EnumValue::EarlyAfterLateShifts]      = ui->label_EarlyAfterLateShifts_Value;
    enumValueLabelValuesMap[EnumValue::ConsecutiveLateShifts]     = ui->label_ConsecutiveLateShifts_Value;
    enumValueLabelValuesMap[EnumValue::DeviationTargetLateShifts] = ui->label_DeviationTargetLateShifts_Value;

    enumValueLabelNumbersMap[EnumValue::ShiftPreferences]          = ui->label_ShiftPreferences_Number;
    enumValueLabelNumbersMap[EnumValue::DayoffPreferences]         = ui->label_DayoffPreferences_Number;
    enumValueLabelNumbersMap[EnumValue::UnassignedShifts]          = ui->label_UnassignedShifts_Number;
    enumValueLabelNumbersMap[EnumValue::LongRests]                 = ui->label_LongRests_Number;
    enumValueLabelNumbersMap[EnumValue::EarlyAfterLateShifts]      = ui->label_EarlyAfterLateShifts_Number;
    enumValueLabelNumbersMap[EnumValue::ConsecutiveLateShifts]     = ui->label_ConsecutiveLateShifts_Number;
    enumValueLabelNumbersMap[EnumValue::DeviationTargetLateShifts] = ui->label_DeviationTargetLateShifts_Number;

    enumValueLabelResultsMap[EnumValue::ShiftPreferences]          = ui->label_ShiftPreferences_Result;
    enumValueLabelResultsMap[EnumValue::DayoffPreferences]         = ui->label_DayoffPreferences_Result;
    enumValueLabelResultsMap[EnumValue::UnassignedShifts]          = ui->label_UnassignedShifts_Result;
    enumValueLabelResultsMap[EnumValue::LongRests]                 = ui->label_LongRests_Result;
    enumValueLabelResultsMap[EnumValue::EarlyAfterLateShifts]      = ui->label_EarlyAfterLateShifts_Result;
    enumValueLabelResultsMap[EnumValue::ConsecutiveLateShifts]     = ui->label_ConsecutiveLateShifts_Result;
    enumValueLabelResultsMap[EnumValue::DeviationTargetLateShifts] = ui->label_DeviationTargetLateShifts_Result;

    refresh();
}

ScoreInfoBoard::~ScoreInfoBoard() {
    delete ui;
}

void ScoreInfoBoard::refresh() {
    // Сброс очков
    for (auto key: enumValueNumbersMap.keys()) {
        enumValueNumbersMap[key] = 0;
    }
    // У каждого водителя 4 ночной смены, но не должно превышать 4
    // TODO: магическое число заменить на соответствующее выражение
    // <кол-во водителей> * 4
    enumValueNumbersMap[EnumValue::DeviationTargetLateShifts] = 44;

    if (schedulerTableModel != nullptr) {
        for (int row = 0; row < schedulerTableModel->rowCount(); row++) {
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                auto index = schedulerTableModel->index(row, column);
                auto textCell = schedulerTableModel->data(index, SchedulerTableModel::WishDayRole).toString();

                // TODO:
                if (textCell == "RR") {
                    enumValueNumbersMap[EnumValue::DayoffPreferences]++;
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
                enumValueNumbersMap[EnumValue::DayoffPreferences] -= numberShiftPreferences;
            } else {
                enumValueNumbersMap[EnumValue::DayoffPreferences] += numberShiftPreferences;
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
                enumValueNumbersMap[EnumValue::ShiftPreferences] += numberShiftPreferencesNight;
            } else {
                enumValueNumbersMap[EnumValue::ShiftPreferences] -= numberShiftPreferencesNight;
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
                enumValueNumbersMap[EnumValue::ShiftPreferences] += numberShiftPreferencesDay;
            } else {
                enumValueNumbersMap[EnumValue::ShiftPreferences] -= numberShiftPreferencesDay;
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
                // TODO: магическое число
                if (numberLateDay > 4) {
                    enumValueNumbersMap[EnumValue::DeviationTargetLateShifts] -= 4;
                    enumValueNumbersMap[EnumValue::DeviationTargetLateShifts] += numberLateDay - 4;
                } else {
                    enumValueNumbersMap[EnumValue::DeviationTargetLateShifts] -= numberLateDay;
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

    //                //                enumValueLabelNumbersMap[EnumValue::UnassignedShifts]
    //            }
    //        }
    //    }

    /* Подсчет очков */
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
