#ifndef SCOREINFOBOARD_H
#define SCOREINFOBOARD_H

#include <QWidget>

namespace Ui {
    class ScoreInfoBoard;
}

#include <QMap>
#include <QLabel>


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

        // Словарь, описывающий тип метрики и количество случаев
        QMap<EnumValue, int> enumValueNumbersMap;

        // Словарь, описывающий тип метрики и виджет для отображения значения очка метрики
        QMap<EnumValue, QLabel*> enumValueLabelValuesMap;

        // Словарь, описывающий тип метрики и виджет для отображения количества случаев
        QMap<EnumValue, QLabel*> enumValueLabelNumbersMap;

        // Словарь, описывающий тип метрики и виджет для отображения суммы очков по метрике
        QMap<EnumValue, QLabel*> enumValueLabelResultsMap;
};

#endif // SCOREINFOBOARD_H
