#ifndef ENUMS_H
#define ENUMS_H


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

// Регистрация типа для использования в QVariant
#include <QVariant>

Q_DECLARE_METATYPE(DayKind)


enum Lines {
    Line_1 = 0,
    Line_2,
    Line_3,
};


#endif // ENUMS_H
