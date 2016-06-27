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

void ScoreInfoBoard::analysis() {
    // Сброс очков
    for (auto key: enumValueDataMap.keys()) {
        enumValueDataMap[key]->number = 0;
    }

    if (schedulerTableModel != nullptr) {
        enumValueDataMap[LongRests]->number = schedulerTableModel->rowCount();
        for (int row = 0; row < schedulerTableModel->rowCount(); row++) {
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                auto index = schedulerTableModel->index(row, column);
                auto textCell = schedulerTableModel->data(index, SchedulerTableModel::WishDayRole).toString();

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

                if (textCell == "RR" && day != DayKind::NONE) {
                    numberShiftPreferences++;
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

                if (textCell == "NN" && isNight(day)) {
                    numberShiftPreferencesNight++;
                }
            }
            // TODO:
            if (numberShiftPreferencesNight > 0) {
                enumValueDataMap[ShiftPreferences]->number += numberShiftPreferencesNight;
            } else {
                enumValueDataMap[ShiftPreferences]->number -= numberShiftPreferencesNight;
            }

            // Количество DayoffPreferences у водителя день
            int numberShiftPreferencesDay = 0;
            for (int column = 0; column < schedulerTableModel->columnCount(); column++) {
                auto index = schedulerTableModel->index(row, column);
                auto day = schedulerTableModel->getDayKind(index);
                auto textCell = schedulerTableModel->data(index, SchedulerTableModel::WishDayRole).toString();

                if (textCell == "DD" && isDay(day)) {
                    numberShiftPreferencesDay++;
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

                if (isNight(day)) {
                    numberLateDay++;
                }
            }

            if (numberLateDay > 0) {
                analysisDeviationTargetLateShifts(numberLateDay);
                analysisConsecutiveLateShifts(row);
                analysisLongRests(row);
                analysisEarlyAfterLateShifts(row);
            }
        }
    }

    if (lineDaysTable != nullptr) {
        for (int row = 0; row < lineDaysTable->rowCount(); row++) {
            for (int column = 0; column < lineDaysTable->columnCount(); column++) {
                auto index =  lineDaysTable->model.index(row,column);
                // Вывести общее количество Day и Night
                auto pair = lineDaysTable->model.get(index);

                if (pair.first != DayKind::NONE) {
                    enumValueDataMap[UnassignedShifts]->number++;
                }

                if (pair.second != DayKind::NONE) {
                    enumValueDataMap[UnassignedShifts]->number++;
                }
            }
        }
    }
}

void ScoreInfoBoard::fillForms() {
    for (auto key: enumValueDataMap.keys()) {
        auto data = enumValueDataMap[key];

        // Заполнение a
        data->labelValue->setText(getFormatFloatValue(key));

        // Заполнение b
        data->labelNumber->setText(getFormatFloatValue(data->number));

        float value = key * data->number;

        // Заполнение c
        data->labelResult->setText(getFormatFloatValue(value));
    }
}

void ScoreInfoBoard::refresh() {
    analysis();
    fillForms();

    auto score = getFormatFloatValue(calcScore());
    ui->label_Score->setText(score);
}

float ScoreInfoBoard::calcScore() {
    float score = 0.0;

    for (auto key: enumValueDataMap.keys()) {
        auto number = enumValueDataMap[key]->number;
        float value = key * number;
        score += value;
    }

    return score;
}
