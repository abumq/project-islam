#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QDir>

namespace {
// Misc
static const QString kDefaultHomeDir = QDir::homePath() + 
        QString(QDir::separator()) + QString(".project-islam") + 
        QString(QDir::separator());
        
} // namespace
#endif // CONSTANTS_H
