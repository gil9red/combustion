#include "enums.h"

bool isDay(DayKind kind) {
    switch (kind) {
        case LINE_1_DAY:
        case LINE_2_DAY:
        case LINE_3_DAY:
            return true;

        default:
            return false;
    }

    return false;
}

bool isNight(DayKind kind) {
    switch (kind) {
        case LINE_1_NIGHT:
        case LINE_2_NIGHT:
        case LINE_3_NIGHT:
            return true;

        default:
            return false;
    }

    return false;
}
