#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <QDir>

namespace {
// Misc
static const std::string kHomeDir = QDir::homePath().toStdString() + QString(QDir::separator()).toStdString() + ".project-islam" + QString(QDir::separator()).toStdString();
} // namespace
#endif // CONSTANTS_H
