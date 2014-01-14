#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QDir>

namespace {
// Misc
static const QString kDefaultHomeDir = QDir::homePath() + 
        QString("/") + QString("project-islam") +
        QString("/");

static const double kDefaultLatitude = -35.281983;
static const double kDefaultLongitude = 149.128718;
} // namespace
#endif // CONSTANTS_H
