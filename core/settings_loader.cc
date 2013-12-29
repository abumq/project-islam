#ifndef _LOGGER
#   define _LOGGER "settings_loader"
#endif
#ifndef _PERFORMANCE_LOGGER
#   define _PERFORMANCE_LOGGER _LOGGER
#endif

#include "settings_loader.h"

#include <QString>
#include <QFile>
#include <QSettings>

#include "core/logging/logging.h"
#include "core/utils/memory.h"
#include "core/constants.h"

const QString SettingsLoader::kMasterSettingsFile = QString("master.settings");
const QString SettingsLoader::kSettingKeyTheme = "theme";
const QString SettingsLoader::kSettingKeyQuranTable = "default_quran";
const QString SettingsLoader::kSettingKeyQuranTranslationTable = "default_quran_translation";
const QString SettingsLoader::kSettingKeyQuranTransliterationTable = "default_quran_transliteration";
const QString SettingsLoader::kSettingKeyQuranTafseerTable = "default_quran_tafseer";
const QString SettingsLoader::kSettingKeyRecitationVolume = "recitation_vol";
const QString SettingsLoader::kSettingKeyDatabaseHost = "db_host";
const QString SettingsLoader::kSettingKeyDatabasePort = "db_port";

QString SettingsLoader::s_defaultHomeDir = QString("");

SettingsLoader::SettingsLoader(const char* settingsFilename) :
    m_settings(nullptr)
{
    initialize(settingsFilename);
}

SettingsLoader::~SettingsLoader()
{
    // Important: Do not trace or log anything here
    // as Easylogging++ repo is already destroyed by this time (because of singleton instance)
    memory::deleteAll(m_settings);
}

void SettingsLoader::initialize(const char* settingsFilename)
{
    _TRACE;
    m_settingsFile = defaultHomeDir() + settingsFilename;
    if (m_settings == nullptr || m_settings->fileName() != m_settingsFile) {
        changeSettingsFile(m_settingsFile);
    }
    // Following log will not log for the very first time
    // because of singleton and log file not initialzed before main() is called
    // so if we see this line in log file, we may have problem because this class should not
    // call this initialze() very often!
    //
    // Important: This is only because we are using '_ELPP_NO_DEFAULT_LOG_FILE'
    LOG_AFTER_N(1, WARNING) << "Created multiple instances (" << ELPP_COUNTER_POS + 1
                            << ") of [SettingsLoader], filename: [" << settingsFilename << "]";
}

SettingsLoader& SettingsLoader::getInstance()
{
    static SettingsLoader loader;
    return loader;
}

void SettingsLoader::saveSettings(QMap<QString, QVariant>* map) const
{
    _TRACE;
    if (m_settings == nullptr || !m_settings->isWritable()) {
        LOG_IF(m_settings != nullptr, ERROR) << "Settings are not writable ["
                                             << m_settings->status() << "]";
    } else {
        LOG(INFO) << "Saving settings to [" << m_settingsFile << "]";
        for (QString setting : map->keys()) {
            VLOG(9) << "Saving setting [" << setting << "]";
            m_settings->setValue(setting, map->value(setting));
        }
    }
}

void SettingsLoader::saveSettings(const QString& key, const QVariant& value) const
{
    _TRACE << "Key: " << key << "; Value: " << value.toString();
    QMap<QString, QVariant> settingsMap;
    settingsMap.insert(key, value);
    saveSettings(&settingsMap);
}

QVariant SettingsLoader::get(const QString& key, const QVariant& defaultValue) const
{
    _TRACE;
    if (m_settings != nullptr) {
        return m_settings->value(key, defaultValue);
    }
    return defaultValue;
}

void SettingsLoader::changeSettingsFile(const QString &filename)
{
    _TRACE;
    LOG(INFO) << "Changing settings file to [" << filename << "]";
    memory::deleteAll(m_settings);
    m_settingsFile = filename;
    bool ok = false;
    if (!QFile::exists(m_settingsFile)) {
        QFile f(m_settingsFile);
        f.open(QFile::ReadWrite);
        if (!f.isOpen()) {
            LOG(ERROR) << "Cannot create file [" << m_settingsFile << "]: " << f.errorString();
        } else {
            ok = true;
        }
    } else {
        ok = true;
    }
    if (ok) {
        m_settings = new QSettings(m_settingsFile, QSettings::IniFormat);
    }
}

QString SettingsLoader::settingsFile() const
{
    return m_settingsFile;
}

QString SettingsLoader::defaultHomeDir()
{
    if (s_defaultHomeDir.isNull() || s_defaultHomeDir.isEmpty()) {
        updateDefaultHomeDir(kDefaultHomeDir);
    }
    return s_defaultHomeDir;
}

void SettingsLoader::updateDefaultHomeDir(const QString& dir)
{
    _TRACE;
    QFile masterSettingsFile(kMasterSettingsFile);
    if (!masterSettingsFile.exists()) {
        // Create master settings
        if (masterSettingsFile.open(QFile::WriteOnly) || masterSettingsFile.isWritable()) {
            masterSettingsFile.write(kDefaultHomeDir.toStdString().c_str());
            masterSettingsFile.flush();
            masterSettingsFile.close();
        } else {
            LOG(ERROR) << "Unable to write to [" << masterSettingsFile.fileName() << "]";
            s_defaultHomeDir = kDefaultHomeDir;
        }
    }
    // Update master settings
    if (!s_defaultHomeDir.isEmpty() && s_defaultHomeDir != dir) {
        if (masterSettingsFile.open(QFile::WriteOnly) || masterSettingsFile.isWritable()) {
            masterSettingsFile.write(dir.toStdString().c_str());
            masterSettingsFile.flush();
            masterSettingsFile.close();
        }
    }
    if (masterSettingsFile.open(QFile::ReadOnly)) {
        QString l = masterSettingsFile.readAll();
        if (!l.isEmpty()) {
            LOG(INFO) << "Setting up home dir to [" << l << "]";
            s_defaultHomeDir = l;
        } else {
            LOG(INFO) << "Setting up home dir to [" << kDefaultHomeDir << "]";
            s_defaultHomeDir = kDefaultHomeDir;
        }
    } else {
        LOG(INFO) << "Setting up home dir to [" << kDefaultHomeDir << "]";
        s_defaultHomeDir = kDefaultHomeDir;
    }
}
