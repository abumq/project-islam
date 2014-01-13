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

double getTimeZone(time_t local_time) {
    tm* tmp = localtime(&local_time);
    tmp->tm_isdst = 0;
    time_t local = mktime(tmp);
    tmp = gmtime(&local_time);
    tmp->tm_isdst = 0;
    time_t gmt = mktime(tmp);
    return (local - gmt) / 3600.0;
}

/* compute local time-zone for a specific date */
double getTimeZone(int year, int month, int day) {
    tm date = { 0 };
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;
    date.tm_isdst = -1;                // determine it yourself from system
    time_t local = mktime(&date);                // seconds since midnight Jan 1, 1970
    return getTimeZone(local);
}
} // namespace datetime
#endif // DATETIME_H
