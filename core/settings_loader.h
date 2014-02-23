#ifndef SETTINGS_LOADER_H
#define SETTINGS_LOADER_H

#include <QVariant>
#include <memory>
#include "core/constants.h"

class QSettings;

template <class Key, class T> class QMap;

class SettingsLoader
{
    static QString s_defaultHomeDir;
public:
    static const QString kSettingKeyTheme;
    static const QString kSettingKeyQuranTable;
    static const QString kSettingKeyQuranTranslationTable;
    static const QString kSettingKeyQuranTransliterationTable;
    static const QString kSettingKeyQuranTafseerTable;
    static const QString kSettingKeyRecitationVolume;
    static const QString kSettingKeyDatabaseHost;
    static const QString kSettingKeyDatabasePort;
    static const QString kLatitudeKey;
    static const QString kLongitudeKey;
    
    static SettingsLoader* getInstance();
    
    void saveSettings(QMap<QString, QVariant>* map);
    void saveSettings(const QString &key, const QVariant &value);
    QVariant get(const QString &key, const QVariant &defaultValue = QVariant()) const;
    
    void updateDefaultHomeDir(const QString&);
    QString defaultHomeDir();
    ~SettingsLoader();
private:
    QString m_settingsFile;
    std::unique_ptr<QSettings> m_settings;
    static const QString kMasterSettingsFile;

    void changeSettingsFile(const QString& filename);
    QString settingsFile() const;
    void initialize(const char* settingsFilename);
    explicit SettingsLoader(const char* settingsFilename = "basic_settings.ini");
    SettingsLoader(const SettingsLoader&);
    SettingsLoader& operator=(const SettingsLoader&);
};

#endif // SETTINGS_LOADER_H
