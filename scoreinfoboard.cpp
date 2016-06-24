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

    enumValueDataMap[ShiftPreferences]          = new ScoreInfoBoard::Data(0, ui->label_ShiftPreferences_Value, ui->label_ShiftPreferences_Number, ui->label_ShiftPreferences_Result);
    enumValueDataMap[DayoffPreferences]         = new ScoreInfoBoard::Data(0, ui->label_DayoffPreferences_Value, ui->label_DayoffPreferences_Number, ui->label_DayoffPreferences_Result);
    enumValueDataMap[UnassignedShifts]          = new ScoreInfoBoard::Data(0, ui->label_UnassignedShifts_Value, ui->label_UnassignedShifts_Number, ui->label_UnassignedShifts_Result);
    enumValueDataMap[LongRests]                 = new ScoreInfoBoard::Data(0, ui->label_LongRests_Value, ui->label_LongRests_Number, ui->label_LongRests_Result);
    enumValueDataMap[EarlyAfterLateShifts]      = new ScoreInfoBoard::Data(0, ui->label_EarlyAfterLateShifts_Value, ui->label_EarlyAfterLateShifts_Number, ui->label_EarlyAfterLateShifts_Result);
    enumValueDataMap[ConsecutiveLateShifts]     = new ScoreInfoBoard::Data(0, ui->label_ConsecutiveLateShifts_Value, ui->label_ConsecutiveLateShifts_Number, ui->label_ConsecutiveLateShifts_Result);
    enumValueDataMap[DeviationTargetLateShifts] = new ScoreInfoBoard::Data(0, ui->label_DeviationTargetLateShifts_Value, ui->label_DeviationTargetLateShifts_Number, ui->label_DeviationTargetLateShifts_Result);

    refresh();
}

ScoreInfoBoard::~ScoreInfoBoard() {
    delete ui;

    // Удаление из словаря и удаление объекта
    for (auto key: enumValueDataMap.keys()) {
        delete enumValueDataMap.take(key);
    }
}

void ScoreInfoBoard::scheduleAnalysis() {
    // Сброс очков
    for (auto key: enumValueDataMap.keys()) {
        enumValueDataMap[key]->number = 0;
    }

    if (schedulerTableModel != nullptr) {
        // У каждого водителя maxGoodDayLateNumber ночной смены, но не должно превышать maxGoodDayLateNumber
        // <кол-во водителей> * maxGoodDayLateNumber
        enumValueDataMap[DeviationTargetLateShifts]->number = schedulerTableModel->rowCount() * maxGoodDayLateNumber;

        for (int row = 0; row < schedulerTableModel->rowCount(); row++) {
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                auto index = schedulerTableModel->index(row, column);
                auto textCell = schedulerTableModel->data(index, SchedulerTableModel::WishDayRole).toString();

                // TODO:
                if (textCell == "RR") {
                    enumValueDataMap[DayoffPreferences]->number++;
                }
            }

            // Количество DayoffPreferences у водителя
            int numberShiftPreferences = 0;
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                auto index = schedulerTableModel->index(row, column);
                auto day = schedulerTableModel->getDayKind(index);
                auto textCell = schedulerTableModel->data(index, SchedulerTableModel::WishDayRole).toString();

                if (textCell == "RR") {
                    if (day != DayKind::NONE) {
                        numberShiftPreferences++;
                    }
                }
            }

            // TODO:
            if (numberShiftPreferences > 0) {
                enumValueDataMap[DayoffPreferences]->number -= numberShiftPreferences;
            } else {
                enumValueDataMap[DayoffPreferences]->number += numberShiftPreferences;
            }

            // Количество ShiftPreferences у водителя ночью
            int numberShiftPreferencesNight = 0;
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                auto index = schedulerTableModel->index(row, column);
                auto day = schedulerTableModel->getDayKind(index);
                auto textCell = schedulerTableModel->data(index, SchedulerTableModel::WishDayRole).toString();

                if (textCell == "NN") {
                    switch(day) {
                        case DayKind::LINE_1_NIGHT:
                        case DayKind::LINE_2_NIGHT:
                        case DayKind::LINE_3_NIGHT:
                            numberShiftPreferencesNight++;
                            break;

                        default:
                            break;
                    }
                }
            }
            // TODO:
            if (numberShiftPreferencesNight > 0) {
                enumValueDataMap[ShiftPreferences] += numberShiftPreferencesNight;
            } else {
                enumValueDataMap[ShiftPreferences] -= numberShiftPreferencesNight;
            }

            // Количество DayoffPreferences у водителя день
            int numberShiftPreferencesDay = 0;
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                auto index = schedulerTableModel->index(row, column);
                auto day = schedulerTableModel->getDayKind(index);
                auto textCell = schedulerTableModel->data(index, SchedulerTableModel::WishDayRole).toString();

                if (textCell == "DD") {
                    switch(day) {
                        case DayKind::LINE_1_DAY:
                        case DayKind::LINE_2_DAY:
                        case DayKind::LINE_3_DAY:
                            numberShiftPreferencesDay++;
                            break;

                        default:
                            break;
                    }
                }
            }
            // TODO:
            if (numberShiftPreferencesDay > 0) {
                enumValueDataMap[ShiftPreferences]->number += numberShiftPreferencesDay;
            } else {
                enumValueDataMap[ShiftPreferences]->number -= numberShiftPreferencesDay;
            }

            // Количество ночных смен у водителя
            int numberLateDay = 0;
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                auto index = schedulerTableModel->index(row, column);
                auto day = schedulerTableModel->getDayKind(index);

                switch(day) {
                    case DayKind::LINE_1_NIGHT:
                    case DayKind::LINE_2_NIGHT:
                    case DayKind::LINE_3_NIGHT:
                        numberLateDay++;
                        break;

                    default:
                        break;
                }
            }

            if (numberLateDay > 0) {
                // TODO:
                if (numberLateDay > maxGoodDayLateNumber) {
                    enumValueDataMap[DeviationTargetLateShifts]->number -= maxGoodDayLateNumber;
                    enumValueDataMap[DeviationTargetLateShifts]->number += numberLateDay - maxGoodDayLateNumber;
                } else {
                    enumValueDataMap[DeviationTargetLateShifts]->number -= numberLateDay;
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
    for (auto key: enumValueDataMap.keys()) {
        auto data = enumValueDataMap[key];

        // Заполнение a
        data->labelValue->setText(getFormatFloatValue(key));

        // Заполнение b
        data->labelNumber->setText(getFormatFloatValue(data->number));

        float value = key * data->number;
        score += value;

        // Заполнение c
        data->labelResult->setText(getFormatFloatValue(value));
    }

    ui->label_Score->setText(getFormatFloatValue(score));
}
