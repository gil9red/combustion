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
        // У каждого водителя maxGoodDayLateNumber ночной смены, но не должно превышать maxGoodDayLateNumber
        // <кол-во водителей> * maxGoodDayLateNumber
        enumValueDataMap[DeviationTargetLateShifts]->number = schedulerTableModel->rowCount() * maxGoodDayLateNumber;

        enumValueDataMap[LongRests]->number = schedulerTableModel->rowCount();

        for (int row = 0; row < schedulerTableModel->rowCount(); row++) {
            analysisDayoffPreferences(row);
            analysisShiftPreferences(row);
            analysisDeviationTargetLateShifts(row);
            analysisConsecutiveLateShifts(row);
            analysisLongRests(row);
            analysisEarlyAfterLateShifts(row);
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
