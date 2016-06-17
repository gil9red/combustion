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


Busman* Busman::fromString(const QString& str) {
    QStringList dataRow = str.split('|');
    dataRow.removeLast();
    dataRow.removeAt(3);

    // Список пожеланий водителей на расписание
    QStringList wishesOnSchedule = getSublist(dataRow, 3);

    // Номер автобуса: A, B, C и т.п.
    QString busNum = dataRow[1];

    Busman* busman = new Busman();
    busman->busNum = busNum;
    busman->wishesOnSchedule = wishesOnSchedule;

    // Выбор линии маршрута водителя
    for (auto c: dataRow[2]) {
        if (c == '1') {
            busman->lines.append(Lines::Line_1);

        } else if (c == '2') {
            busman->lines.append(Lines::Line_2);

        } else if (c == '3') {
            busman->lines.append(Lines::Line_3);
        }
    }

    // Отсортировка списка линий
    std::sort(busman->lines.begin(), busman->lines.end());

    return busman;
}
