#include "parserpuzzlefile.h"

#include <QFile>
#include <QDebug>


ParserPuzzleFile::ParserPuzzleFile()
{

}


QStringList ParserPuzzleFile::getSublist(QStringList list, int begin, int end) {
    if (begin > end) {
        qWarning() << "Начальный индекс больше конечного.";
        return QStringList();
    }

    QStringList subList;
    for (int i = begin; i <= end; i++) {
        subList.append(list.at(i));
    }

    return subList;
}


QStringList ParserPuzzleFile::getSublist(QStringList list, int begin) {
    return ParserPuzzleFile::getSublist(list, begin, list.size() - 1);
}


void ParserPuzzleFile::parse(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Не найден файл:" << fileName;
        return;
    }

    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }

    // Получение списка дат (3 строка)
    QStringList days = lines.at(2).split('|');
    days.removeFirst();
    days.removeLast();

    // Получение списка водителей и их предпочтений
    int indexEndTable = -1;
    for (int i = 3; i < lines.size(); i++) {
        QString line = lines.at(i);

        if (line.startsWith("---------")) {
            indexEndTable = i;
            break;
        }

        QStringList dataRow = line.split('|');
        dataRow.removeLast();
        dataRow.removeAt(3);

        // Список пожеланий водителей на расписание
        QStringList wishesOnSchedule = ParserPuzzleFile::getSublist(dataRow, 3);
        if (days.size() != wishesOnSchedule.size()) {
            qCritical() << "Общее количество дней и количество дней в пожеланиях водителей в расписании количествено отличаются.";
            return;
        }

        // Номер автобуса: A, B, C и т.п.
        QString busNum = dataRow[1];

        // Выбор линии маршрута водителя
        QString selectLines = dataRow[2].remove('0');
        qDebug() << dataRow[0] << busNum << selectLines << wishesOnSchedule << wishesOnSchedule.size();
    }

    if (indexEndTable == -1) {
        qWarning() << "Индекс конца таблицы неизвестен. Что-то пошло не так.";
        return;
    }

    // Получаем строку с SCORE и вытаскиваем его значение
    QString score = lines.at(indexEndTable + 1).split(":")[1];
    qDebug() << score;

    QMap<QString, QString> valueDescriptionMap;
    // Перебор обозначений в таблице
    for (int i = indexEndTable + 3; i < lines.size(); i++) {
        QString line = lines.at(i);
        QStringList temps = line.split(" - ");

        valueDescriptionMap[temps[0]] = temps[1];
    }

    QMapIterator<QString, QString> it(valueDescriptionMap);
    while (it.hasNext()) {
        it.next();
        qDebug() << QString("%1: %2").arg(it.key()).arg(it.value());
    }

//    qDebug();
//    foreach (QString line, lines) {
//        qDebug() << line;
//    }
}
