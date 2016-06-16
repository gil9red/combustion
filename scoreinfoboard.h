#ifndef SCOREINFOBOARD_H
#define SCOREINFOBOARD_H

#include <QWidget>

namespace Ui {
    class ScoreInfoBoard;
}

class ScoreInfoBoard : public QWidget
{
        Q_OBJECT

    public:
        explicit ScoreInfoBoard(QWidget *parent = 0);
        ~ScoreInfoBoard();

        void refresh();

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

    private:
        Ui::ScoreInfoBoard *ui;
};

#endif // SCOREINFOBOARD_H
