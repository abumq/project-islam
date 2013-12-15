#ifndef UTILS_H
#define UTILS_H

#include <QDir>
#include <QString>

namespace utils {

inline QString buildPath(const QStringList& pathElements) {
    return pathElements.join(QDir::separator()).append(QDir::separator());
}

inline QString buildFilename(const QStringList& pathElements) {
    return pathElements.join(QDir::separator());
}

inline QString buildFilename(const QStringList& pathElements, 
                             const QString& filename) {
    return pathElements.join(QDir::separator()).append(QDir::separator()) + filename;
}

}

#endif // UTILS_H
