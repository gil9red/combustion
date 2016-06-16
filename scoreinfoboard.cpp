#include "scoreinfoboard.h"
#include "ui_scoreinfoboard.h"


QString getFormatFloatValue(float value) {
    return QString::number(value,  'f',  1);
}


ScoreInfoBoard::ScoreInfoBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScoreInfoBoard)
{
    // TODO: добавить total result
    ui->setupUi(this);

    refresh();
}

ScoreInfoBoard::~ScoreInfoBoard()
{
    delete ui;
}

void ScoreInfoBoard::refresh() {
    // TODO: названия виджетов соответственно их перечислению
    ui->label_2->setText(getFormatFloatValue(EnumValue::ShiftPreferences));
    ui->label_13->setText(getFormatFloatValue(EnumValue::DayoffPreferences));
    ui->label_18->setText(getFormatFloatValue(EnumValue::UnassignedShifts));
    ui->label_23->setText(getFormatFloatValue(EnumValue::LongRests));
    ui->label_26->setText(getFormatFloatValue(EnumValue::EarlyAfterLateShifts));
    ui->label_33->setText(getFormatFloatValue(EnumValue::ConsecutiveLateShifts));
    ui->label_38->setText(getFormatFloatValue(EnumValue::DeviationTargetLateShifts));
}
