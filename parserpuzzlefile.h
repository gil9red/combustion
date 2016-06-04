#ifndef PARSERPUZZLEFILE_H
#define PARSERPUZZLEFILE_H


#include <QString>
#include <QMap>

#include "busman.h"

// Класс для разбора файлов паззла
/*
Пример содержимого файла:

GIVEN PUZZLE
---------------------------------------------------+
_________|01|02|03|04|05|06|07|08|09|10|11|12|13|14|
01|A|100||RR|NN|XX|XX|00|00|RR|00|00|XX|XX|NN|00|00|
02|B|103||00|RR|00|XX|XX|DD|RR|DD|00|00|XX|XX|00|RR|
03|C|020||00|NN|RR|NN|XX|XX|DD|00|NN|RR|00|XX|XX|00|
04|D|020||DD|RR|RR|DD|00|XX|XX|00|00|NN|RR|NN|XX|XX|
05|E|023||XX|NN|00|RR|RR|00|XX|XX|00|RR|NN|00|00|XX|
06|F|120||XX|XX|00|RR|RR|00|00|XX|XX|00|RR|DD|DD|00|
07|G|003||NN|XX|XX|DD|00|RR|00|00|XX|XX|DD|RR|RR|00|
08|H|020||RR|00|XX|XX|NN|RR|NN|00|00|XX|XX|00|00|DD|
09|I|003||RR|RR|00|XX|XX|NN|00|RR|DD|NN|XX|XX|00|RR|
10|J|003||00|RR|00|00|XX|XX|00|RR|00|00|NN|XX|XX|DD|
11|K|103||RR|RR|00|NN|00|XX|XX|00|NN|RR|00|00|XX|XX|
---------------------------------------------------+
SCORE:-1857(-1049.15%)
---------------------
00 - VOID
XX - STATIC DAY OFF
RR - REST PREFERENCE
DD - DAY SHIFT PREFERENCE
NN - NIGHT SHIFT PREFERENCE
*/
class ParserPuzzleFile
{
    public:
        ParserPuzzleFile();
        static QList<Busman*> parse(const QString& fileName, QMap<QString, QString>& valueDescriptionMap) throw(std::exception);
};

#endif // PARSERPUZZLEFILE_H
