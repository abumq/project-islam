#include "settings_loader.h"
#include <QFile>
#include <QSettings>
#include "core/logging.h"
#include "core/constants.h"

SettingsLoader::SettingsLoader()
{
    m_settingsFile = QString(kHomeDir.c_str()) + "basic_settings.ini";
    m_settings = nullptr;
    changeSettingsFile(m_settingsFile);
}

SettingsLoader::~SettingsLoader()
{
    if (m_settings != nullptr) {
        delete m_settings;
        m_settings = nullptr;
    }
}

void SettingsLoader::saveSettings(QMap<QString, QVariant>* map)
{
    if (m_settings == nullptr || !m_settings->isWritable()) {
        LOG(ERROR) << "Settings are not writable [" << m_settings->status() << "]";
    } else {
        LOG(INFO) << "Saving settings to [" << m_settingsFile << "]";
        for (QString setting : map->keys()) {
            VLOG(9) << "Saving setting [" << setting << "]";
            m_settings->setValue(setting, map->value(setting));
        }
    }
}

QVariant SettingsLoader::get(const QString& key, const QVariant& defaultValue) const
{
    if (m_settings != nullptr) {
        return m_settings->value(key, defaultValue);
    }
    return defaultValue;
}

void SettingsLoader::changeSettingsFile(const QString &filename)
{
    if (m_settings != nullptr) {
        delete m_settings;
        m_settings = nullptr;
    }
    m_settingsFile = filename;
    bool okToAlloc = false;
    if (!QFile::exists(m_settingsFile)) {
        QFile f(m_settingsFile);
        f.open(QFile::ReadWrite);
        if (!f.isOpen()) {
            LOG(ERROR) << "Cannot create file [" << m_settingsFile << "]";
        } else {
            okToAlloc = true;
        }
    } else {
        okToAlloc = true;
    }
    if (okToAlloc) {
        m_settings = new QSettings(m_settingsFile, QSettings::NativeFormat);
    }
}

QString SettingsLoader::settingsFile() const
{
    return m_settingsFile;
}
