#ifndef SCOREINFOBOARD_H
#define SCOREINFOBOARD_H

#include <QWidget>

namespace Ui {
    class ScoreInfoBoard;
}

#include <QMap>
#include <QLabel>
#include "scheduler_table_model.h"
#include "linedaystable.h"


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

        SchedulerTableModel* schedulerTableModel = nullptr;
        LineDaysTable* lineDaysTable = nullptr;

    private:
        Ui::ScoreInfoBoard *ui;

        // TODO: словари можно объединить в один -- создать структуру и в ней описать значения
        // которые в словарях описаны
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
