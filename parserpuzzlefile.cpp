#include "parserpuzzlefile.h"

#include <stdexcept>

#include <QFile>
#include <QDebug>


ParserPuzzleFile::ParserPuzzleFile() {

}

QList<Busman*> ParserPuzzleFile::parse(const QString& fileName, QMap<QString, QString>& valueDescriptionMap) throw(std::exception) {
    QList<Busman*> busmanList;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::logic_error(QString("Не найден файл: %1.").arg(fileName).toStdString());
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

        // Если дошли до конца таблицы
        if (line.startsWith("---------")) {
            indexEndTable = i;
            break;
        }

        Busman* busman = Busman::fromString(line, days);
        busmanList.append(busman);
    }

    if (indexEndTable == -1) {
        throw std::logic_error(QString("Индекс конца таблицы неизвестен. Что-то пошло не так.").toStdString());
    }

//    // Получаем строку с SCORE и вытаскиваем его значение
//    QString score = lines.at(indexEndTable + 1).split(":")[1];
//    qDebug() << score;

////    QMap<QString, QString> valueDescriptionMap;
//    valueDescriptionMap.clear();
//    // Перебор обозначений в таблице
//    for (int i = indexEndTable + 3; i < lines.size(); i++) {
//        QString line = lines.at(i);
//        QStringList temps = line.split(" - ");

//        valueDescriptionMap[temps[0]] = temps[1];
//    }

//    QMapIterator<QString, QString> it(valueDescriptionMap);
//    while (it.hasNext()) {
//        it.next();
//        qDebug() << QString("%1: %2").arg(it.key()).arg(it.value());
//    }

////    qDebug();
////    foreach (QString line, lines) {
////        qDebug() << line;
////    }
    return busmanList;
}
