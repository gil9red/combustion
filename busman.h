#ifndef BUSMAN_H
#define BUSMAN_H


#include <QString>
#include <QStringList>


// Класс, описывающий водителя автобуса
class Busman
{
    public:
        Busman();

        // Функция парсит указанную строку и пытается создать из нее объект Busman
        static Busman* fromString(const QString& str, const QStringList & days);

    public:
        QString busNum;
        QString selectLines;
        QStringList wishesOnSchedule;

        // Список рабочих дней
        QStringList workingDays;
};


// Регистрация типа для использования в QVariant
#include <QVariant>

Q_DECLARE_METATYPE(Busman*)


#endif // BUSMAN_H
