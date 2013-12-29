#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QDir>

namespace {
// Misc
static const QString kDefaultHomeDir = QDir::homePath() + 
        QString("/") + QString("project-islam") +
        QString("/");
        
} // namespace
#endif // CONSTANTS_H
