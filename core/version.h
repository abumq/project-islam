#ifndef VERSION_H
#define VERSION_H

#include <QString>

namespace version {

const int kMajorVersion = 1;
const int kMinorVersion = 0;
const int kPatchVersion = 0;
//const char* kTag = "a";

/**
 * @brief versionString Formats current version and returns
 * @return String like 1.0.0a
 */
inline QString versionString() {
    return QString::number(kMajorVersion) + 
    "." + QString::number(kMinorVersion) + 
    "." + QString::number(kPatchVersion)/* +
    QString(kTag)*/;
}

/**
 * @brief isCurrentVersion Whether or not specified versionStr represents
 * current version of app.
 */
inline bool isCurrentVersion(const QString& versionStr) {
    return versionString() == versionStr;
}

} // namespace version

#endif // VERSION_H
