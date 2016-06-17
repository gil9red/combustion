#include "parserpuzzlefile.h"

#include <stdexcept>

#include <QFile>
#include <QDebug>


// TODO:
QList<Busman*> ParserPuzzleFile::parse(const QString& fileName) throw(std::exception) {
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

    // Получение списка водителей и их предпочтений
    int indexEndTable = -1;
    for (int i = 3; i < lines.size(); i++) {
        QString line = lines.at(i);

        // Если дошли до конца таблицы
        if (line.startsWith("---------")) {
            indexEndTable = i;
            break;
        }

        Busman* busman = Busman::fromString(line);
        busmanList.append(busman);
    }

    if (indexEndTable == -1) {
        throw std::logic_error(QString("Индекс конца таблицы неизвестен. Что-то пошло не так.").toStdString());
    }

    return busmanList;
}
