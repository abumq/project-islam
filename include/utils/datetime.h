#ifndef DATETIME_H
#define DATETIME_H

#include <QString>

namespace datetime {

inline QString formatMillisecondsAsDuration(qint64 ms) {
    return QString::number(ms / 1000) + "s";
}

} // namespace datetime
#endif // DATETIME_H
