#include "busman.h"
#include <stdexcept>
#include <QDebug>


QStringList getSublist(QStringList list, int begin, int end) {
    if (begin > end) {
        throw std::range_error("Начальный индекс больше конечного.");
    }

    QStringList subList;
    for (int i = begin; i <= end; i++) {
        subList.append(list.at(i));
    }

    return subList;
}


QStringList getSublist(QStringList list, int begin) {
    return getSublist(list, begin, list.size() - 1);
}


//Busman::Busman() {

//}

Busman* Busman::fromString(const QString& str, const QStringList& days) {
    QStringList dataRow = str.split('|');
    dataRow.removeLast();
    dataRow.removeAt(3);

    // Список пожеланий водителей на расписание
    QStringList wishesOnSchedule = getSublist(dataRow, 3);

    // Номер автобуса: A, B, C и т.п.
    QString busNum = dataRow[1];

    // Выбор линии маршрута водителя
    QString selectLines = dataRow[2].remove('0');

    qDebug() << dataRow[0] << busNum << selectLines << wishesOnSchedule << wishesOnSchedule.size();

    Busman* busman = new Busman();
    busman->busNum = busNum;
    busman->wishesOnSchedule = wishesOnSchedule;
    busman->selectLines = selectLines;
    return busman;
}
