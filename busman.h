#ifndef BUSMAN_H
#define BUSMAN_H


#include <QString>
#include <QStringList>
#include <QMap>

#include "enums.h"


// Класс, описывающий водителя автобуса
class Busman
{
    public:
        // Функция парсит указанную строку и пытается создать из нее объект Busman
        static Busman* fromString(const QString& str);

    public:
        QString busNum;
        QList<Lines> lines;
        QStringList wishesOnSchedule;
        QList<DayKind> workingDays;
};


// Регистрация типа для использования в QVariant
#include <QVariant>

Q_DECLARE_METATYPE(Busman*)



#endif // BUSMAN_H
