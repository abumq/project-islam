#ifndef SETTINGS_LOADER_H
#define SETTINGS_LOADER_H

#include <QString>
#include <QVariant>
#include <QFile>
#include "core/constants.h"

class QSettings;

template <class Key, class T> class QMap;

class SettingsLoader
{
public:
    SettingsLoader();
    ~SettingsLoader();
    void saveSettings(QMap<QString, QVariant>* map);
    QVariant get(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void changeSettingsFile(const QString& filename);
    QString settingsFile() const;
    void initialize();
    static QString defaultHomeDir();
    static void updateDefaultHomeDir(const QString&);
private:
    QString m_settingsFile;
    QSettings* m_settings;
    
    static const QString kMasterSettingsFile;
    static QString s_defaultHomeDir;
};

#endif // SETTINGS_LOADER_H
