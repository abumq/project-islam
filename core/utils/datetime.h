#ifndef DATETIME_H
#define DATETIME_H

#include <QString>

namespace datetime {

inline QString formatMillisecondsAsDuration(qint64 ms) {
    return QString::number(ms / 1000) + "s";
}

double georgianToJulianDate(int year, int month, int day) {
    // See http://quasar.as.utexas.edu/BillInfo/JulianDatesG.html
    const float kAverageLengthOfYear = 365.25f;
    const float kAverageLengthOfMonth = 30.4375f;
    if (month < 3) {
        year -= 1;
        month += 12;
    }
    double a = floor(year / 100.0);
    double b = 2 - a + floor(a / 4.0);
    return floor(kAverageLengthOfYear * (year + 4716))
            + floor(kAverageLengthOfMonth * (month + 1))
            + day + b - 1524.5;
}
} // namespace datetime
#endif // DATETIME_H
