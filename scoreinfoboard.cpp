#include "scoreinfoboard.h"
#include "ui_scoreinfoboard.h"
#include <QDebug>


QString getFormatFloatValue(float value) {
    return QString::number(value,  'f',  1);
}


ScoreInfoBoard::ScoreInfoBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScoreInfoBoard)
{
    ui->setupUi(this);

    enumValueNumbersMap[EnumValue::ShiftPreferences]          = 0;
    enumValueNumbersMap[EnumValue::DayoffPreferences]         = 0;
    enumValueNumbersMap[EnumValue::UnassignedShifts]          = 0;
    enumValueNumbersMap[EnumValue::LongRests]                 = 0;
    enumValueNumbersMap[EnumValue::EarlyAfterLateShifts]      = 0;
    enumValueNumbersMap[EnumValue::ConsecutiveLateShifts]     = 0;
    enumValueNumbersMap[EnumValue::DeviationTargetLateShifts] = 0;

//    // TODO: remove
//    int i = 0;
//    for (auto key: enumValueNumbersMap.keys()) {
//        enumValueNumbersMap[key] = ++i;
//    }

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

ScoreInfoBoard::~ScoreInfoBoard()
{
    delete ui;
}

void ScoreInfoBoard::refresh() {
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
