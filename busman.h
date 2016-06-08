#ifndef BUSMAN_H
#define BUSMAN_H


#include <QString>
#include <QStringList>
#include <QMap>


// Класс, описывающий водителя автобуса
class Busman
{
    public:
//        Busman();

        // Функция парсит указанную строку и пытается создать из нее объект Busman
        static Busman* fromString(const QString& str, const QStringList & days);

    public:
        QString busNum;
        QString selectLines;
        QStringList wishesOnSchedule;

        // Список рабочих дней
        // Варианты значения для рабочего дня:
        // TODO: название лучше более смысловое
        enum DayKind {
            LINE_1_DAY,
            LINE_1_NIGHT,
            LINE_2_DAY,
            LINE_2_NIGHT,
            LINE_3_DAY,
            LINE_3_NIGHT,
        };
        QMap<int, DayKind> workingDays;
};


// Регистрация типа для использования в QVariant
#include <QVariant>

Q_DECLARE_METATYPE(Busman*)


#endif // BUSMAN_H
