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
    static QString s_defaultHomeDir;
public:
    
    static const QString kSettingKeyTheme;
    static const QString kSettingKeyQuranTranslationTable;
    static const QString kSettingKeyQuranTranslationFile;
    static const QString kSettingKeyRecitationVolume;
    
    explicit SettingsLoader(const char* settingsFilename = "basic_settings.ini");
    ~SettingsLoader();
    void saveSettings(QMap<QString, QVariant>* map) const;
    void saveSettings(const QString &key, const QVariant &value) const;
    QVariant get(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void changeSettingsFile(const QString& filename);
    QString settingsFile() const;
    void initialize(const char* settingsFilename);
    QString defaultHomeDir();
    void updateDefaultHomeDir(const QString&);
private:
    QString m_settingsFile;
    QSettings* m_settings;
    
    static const QString kMasterSettingsFile;
};

#endif // SETTINGS_LOADER_H
