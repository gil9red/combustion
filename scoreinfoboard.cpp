#include "scoreinfoboard.h"
#include "ui_scoreinfoboard.h"

ScoreInfoBoard::ScoreInfoBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScoreInfoBoard)
{
    ui->setupUi(this);
}

ScoreInfoBoard::~ScoreInfoBoard()
{
    delete ui;
}
