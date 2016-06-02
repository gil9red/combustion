#include "parserpuzzlefile.h"

#include <QFile>
#include <QDebug>


ParserPuzzleFile::ParserPuzzleFile()
{

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

    qDebug() << lines.at(2);
    qDebug() << lines.at(2).split('|');

//    foreach (QString line, lines) {
//        qDebug() << line;
//    }
}
