#ifndef UTILS_H
#define UTILS_H

#include <QDir>
#include <QString>

namespace filesystem {

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

} // namespace filesystem

#endif // UTILS_H
