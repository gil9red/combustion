#ifndef BUSMAN_H
#define BUSMAN_H


#include <QString>
#include <QStringList>
#include <QMap>


// Класс, описывающий водителя автобуса
class Busman
{
    public:
        // Функция парсит указанную строку и пытается создать из нее объект Busman
        static Busman* fromString(const QString& str);

    public:
        QString busNum;

        // TODO: наверное, лучше использовать список BusmanTableModel::Lines
        // TODO: отсортировать номера линий
        QString selectLines;
        QStringList wishesOnSchedule;

        // TODO: мне кажется, это перечисление или должно быть глобальным -- в отдельном файле,
        // или принадлежать модели -- больше подходит.
        // Список рабочих дней
        // Варианты значения для рабочего дня:
        // TODO: название лучше более смысловое
        enum DayKind {
            NONE = 0,
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
Q_DECLARE_METATYPE(Busman::DayKind)


#endif // BUSMAN_H
