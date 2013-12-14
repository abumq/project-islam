#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QStringList>

namespace version {

const int kMajorVersion = 0;
const int kMinorVersion = 0;
const int kPatchVersion = 1;

/**
 * @brief versionString Formats current version and returns
 * @return String like 1.0.0
 */
inline QString versionString() {
    return QString::number(kMajorVersion) + 
            "." + QString::number(kMinorVersion) + 
            "." + QString::number(kPatchVersion);
}

/**
 * @brief isCurrentVersion Whether or not specified versionStr represents
 * current version of app.
 * @param versionStr A string in format x.x.x
 */
inline bool isCurrentVersion(const QString& versionStr) {
    QStringList sep = versionStr.split(".");
    return sep[0].toInt() == kMajorVersion && 
            sep[1].toInt() == kMinorVersion && 
            sep[2].toInt() == kPatchVersion;
}

} // namespace version

#endif // VERSION_H
