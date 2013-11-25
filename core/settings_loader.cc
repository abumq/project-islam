#include "settings_loader.h"
#include <QString>
#include <QFile>
#include <QSettings>
#include "core/logging.h"
#include "core/constants.h"

const QString SettingsLoader::kMasterSettingsFile = QString("master.settings");
QString SettingsLoader::s_defaultHomeDir = QString();

SettingsLoader::SettingsLoader()
{
    initialize();
}

SettingsLoader::~SettingsLoader()
{
    if (m_settings != nullptr) {
        delete m_settings;
        m_settings = nullptr;
    }
}

void SettingsLoader::initialize()
{
    m_settingsFile = defaultHomeDir() + "basic_settings.ini";
    m_settings = nullptr;
    changeSettingsFile(m_settingsFile);
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

QString SettingsLoader::defaultHomeDir()
{

    
    /*
     
    // FIXME: Uncommenting this code causes segmentation fault on Intel C++
    //        I think it's to do with order of initialization of static member
    //        < This code is actually what should be used, hence need fixed >
     
    if (s_defaultHomeDir.isEmpty()) {
        updateDefaultHomeDir(kDefaultHomeDir);
    }
    return s_defaultHomeDir;
    
    */
    return QDir::homePath() +  QString(QDir::separator()) + QString(".project-islam") + QString(QDir::separator());
}

void SettingsLoader::updateDefaultHomeDir(const QString& dir)
{
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
    } else {
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
}
