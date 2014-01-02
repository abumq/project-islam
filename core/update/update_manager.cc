#ifndef _LOGGER
#   define _LOGGER "update_manager"
#endif
#ifndef _PERFORMANCE_LOGGER
#   define _PERFORMANCE_LOGGER _LOGGER
#endif

#include "core/update/update_manager.h"

#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QtConcurrent/QtConcurrent>
#include <QJsonDocument>
#include <QPluginLoader>
#include <QNetworkReply>
#include <QProcess>

#include "core/utils/memory.h"
#include "core/utils/version.h"
#include "core/utils/filesystem.h"
#include "core/settings_loader.h"
#include "core/extension/extension_loader.h"
#include "core/extension/extension_base.h"
#include "core/extension/abstract_extension.h"
#include "core/extension/extension_info.h"
#include "core/extension/extension_bar.h"
#include "core/logging/logging.h"
#include "core/data/database_manager.h"

const char* UpdateManager::kServerUrlBase = "http://www.icplusplus.com";
const char* UpdateManager::kVersionInfoFilename = "tools/project-islam/vinfo.txt";

const char* UpdateManager::kRemoteFilesSuffix = ".zip";
const char* UpdateManager::kLocalFilesSuffix = ".upgrade";

UpdateManager::UpdateManager(QObject *parent) :
    DownloadManager(parent)
{
    _TRACE;
    // Load m_lastChecked
    const QDate defaultDate = QDate::currentDate().addDays(-1);
    QString lastCheckedStr = 
            SettingsLoader::getInstance()->get("update_checked", 
                                              QVariant(defaultDate.toString())).toString();
    m_lastChecked = QDate::fromString(lastCheckedStr);
    if (m_lastChecked.isNull() || !m_lastChecked.isValid()) {
        m_lastChecked = defaultDate;
    }
    SettingsLoader::getInstance()->saveSettings("update_checked", QVariant(m_lastChecked.toString()));
}

UpdateManager::~UpdateManager()
{
    _TRACE;
    if (m_future.isRunning()) {
        LOG(WARNING) << "Updater was running while killed process.";
        m_futureWatcher.cancel();
        // Ref: http://qt-project.org/doc/qt-5.1/qtcore/qfuturewatcher.html#cancel
        m_futureWatcher.waitForFinished();
        m_futureWatcher.thread()->quit();
    }
}

void UpdateManager::initialize(ExtensionBar* extensionBar)
{
    _TRACE;
    m_extensionBar = CHECK_NOTNULL(extensionBar);
    QObject::connect(this, SIGNAL(downloadProgress(qint64,qint64)), 
                     this, SLOT(downloadProgressUpdated(qint64,qint64)));
    // Update timer
    m_updateTimer.setInterval(kCheckIntervalInMs);
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(performAsyncUpdate()));
    // Single shot anyway!
    performAsyncUpdate();
    m_updateTimer.start(kCheckIntervalInMs);
}

bool UpdateManager::updateFiles()
{
    // Check to see if upgrade.info exist,
    // if yes launch Upgrade binary with arguments
    // to this application to start detached process
    // with same arguments - Do not manually restart here!
    // This will not work
    QFile upgradeInfoFile("upgrade.info");
    if (upgradeInfoFile.exists() 
            && upgradeInfoFile.open(QIODevice::ReadOnly) 
            && upgradeInfoFile.size() > 0) {
        upgradeInfoFile.close();
#if defined(Q_OS_UNIX)
        QFile upgradeExec(qApp->applicationDirPath() + "/upgrade-lc-now");
#elif defined(Q_OS_WIN)
        QFile upgradeExec(qApp->applicationDirPath() + "/upgrade-lc-now.exe");
#endif // defined(Q_OS_UNIX)
        if (upgradeExec.exists()) {
            QStringList arguments;
            QFileInfo upgradeInfoFileInfo("upgrade.info");
            arguments.append("--upgrade-file");
            arguments.append(upgradeInfoFileInfo.filePath());
            arguments.append("--app");
            arguments.append(qApp->applicationFilePath());
            arguments.append(qApp->arguments());
            m_performedUpdate = true;
            QProcess::startDetached(upgradeExec.fileName(), arguments);
            return true;
        }
    }
    m_performedUpdate = false;
    return false;
}

void UpdateManager::saveDownloadedFilesListToFile() const
{
    QFile f("upgrade.info");
    if (!f.open(QIODevice::WriteOnly)) {
        LOG(ERROR) << "Unable to create [upgrade.info]";
    } else {
        for (QString filename : m_downloadedFiles) {
            f.write(QString(filename + "\n").toStdString().c_str());
            f.flush();
        }
        f.close();
    }
}

bool UpdateManager::needToCheckForUpdates() const
{
    _TRACE;
    return m_lastChecked.daysTo(QDate::currentDate()) >= kDaysToCheck;
}

QString UpdateManager::versionInfoUrl() const
{
    _TRACE;
    return QString(kServerUrlBase) + "/" + QString(kVersionInfoFilename);
}

bool UpdateManager::update()
{
    _TRACE;
    if (!needToCheckForUpdates()) {
        LOG(DEBUG) << "Ignorning updater";
        return true;
    }
    TIMED_SCOPE(timer, "Update");
    
    bool result = false;
    LOG(INFO) << "Checking for updates...";
    
    bool downloadOk = false;
    QString jsonVInfo = QString(downloadBytes(versionInfoUrl(), &downloadOk));
    if (downloadOk) {
        LOG(DEBUG) << "VInfo downloaded from server: " << std::endl << jsonVInfo;
        // Deserialize json
        QJsonParseError jsonError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonVInfo.toUtf8(), &jsonError);
        if (jsonError.error == QJsonParseError::NoError) {
            QJsonObject jsonObject = jsonDocument.object();
            QString currOs;
#if defined(Q_OS_LINUX)
            currOs = "linux";
#elif defined(Q_OS_WIN)
            currOs = "win";
#elif defined(Q_OS_MAC)
            currOs = "mac";
#else
            LOG(ERROR) << "Operating system not supported for update";
            return false;
#endif // defined(Q_OS_LINUX)
            QJsonObject osObj = jsonObject[currOs].toObject();
            if (osObj["available"].toBool()) {
                result = updatePlatform(&osObj);
                result = result && updateDatabase(&osObj);
                result = result && updateExtensions(&osObj);
                if (result) {
                    // Everything updated successfully! Let's save it to file
                    saveDownloadedFilesListToFile();
                }
            } else {
                LOG(ERROR) << "No downloadable update available for [" 
                           << currOs << "]";
            }
        } else {
            LOG(ERROR) << "Error while parsing json. Error [" 
                       << jsonError.errorString() << "]";
        }
    }
    if (result) {
        m_lastChecked = QDate::currentDate();
        SettingsLoader::getInstance()->saveSettings("update_checked", QVariant(m_lastChecked.toString()));
    }
    return result;
}

bool UpdateManager::updatePlatform(QJsonObject* jsonObject)
{    
    _TRACE;
    QJsonObject platformObj = (*jsonObject)["platform"].toObject();
    QString ver = platformObj["version"].toString();
    bool forceUpdate = platformObj["force_update"].toBool();
    bool startUpgrade = !version::isCurrentVersion(ver) || forceUpdate;
    bool result = true; // We will mark it false if download fail
    if (startUpgrade) {
        LOG(INFO) << "Upgrading platform from [" << version::versionString() 
                  << "] to [" << ver << "]" << (forceUpdate ? " (FORCED)" : "");
        QString baseUrl = platformObj["base"].toString();
        QStringList filesList = platformObj["files"].toString().split(',');
        for (QString filename : filesList) {
            
            QString targetDir = filesystem::buildPath(QStringList() << qApp->applicationDirPath());
            QString tempFilename = targetDir + filename + QString(kLocalFilesSuffix);
            LOG(INFO) << "Downloading platform file [" 
                      << filename << "] from [" + baseUrl + "] to [" << targetDir << "]";
            result = result && downloadFile(baseUrl + filename, tempFilename);
            if (result) {
                m_downloadedFiles.push_back(tempFilename);
            }
        }
    } else {
        LOG(INFO) << "Platform is up to date!";
    }
    return result;
}

bool UpdateManager::updateDatabase(QJsonObject* jsonObject)
{
    _TRACE;
    QJsonObject databaseObj = (*jsonObject)["database"].toObject();
    QString ver = databaseObj["version"].toString();
    QString currVer = SettingsLoader::getInstance()->get("curr_db_ver", QVariant(QString("1.0.0"))).toString();
    bool forceUpdate = databaseObj["force_update"].toBool();
    bool startUpgrade = currVer != ver || forceUpdate;
    bool result = true; // We will mark it false if download fail
    if (startUpgrade) {
        LOG(INFO) << "Upgrading database from [" << currVer 
                  << "] to [" << ver << "]" << (forceUpdate ? " (FORCED)" : "");
        QString baseUrl = databaseObj["base"].toString();
        QStringList filesList = databaseObj["files"].toString().split(',');
        for (QString filename : filesList) {
            QString targetDir = filesystem::buildPath(QStringList() << SettingsLoader::getInstance()->defaultHomeDir() << "data");
            QString tempFilename = targetDir + filename + QString(kLocalFilesSuffix);
            LOG(INFO) << "Downloading database file [" 
                      << filename << "] from [" + baseUrl + "] to [" << targetDir << "]";
            result = result && downloadFile(baseUrl + filename, tempFilename);
            if (result) {
                m_downloadedFiles.push_back(tempFilename);
            }
        }
    } else {
        LOG(INFO) << "Database is up to date!";
    }
    SettingsLoader::getInstance()->saveSettings("curr_db_ver", QVariant(ver));
    return result;
}

bool UpdateManager::updateExtensions(QJsonObject* jsonObject)
{
    _TRACE;
    QJsonArray extensionsArr = (*jsonObject)["extensions"].toArray();
    bool result = true; // We will mark it false if download fail
    foreach (const QJsonValue& value, extensionsArr) {
        QJsonObject extensionobj = value.toObject();
        QString name = extensionobj["name"].toString();
        AbstractExtension* ex = m_extensionBar->hasExtension(name);
        if (ex != nullptr) {
            QString ver = extensionobj["version"].toString();
            bool forceUpdate = extensionobj["force_update"].toBool();
            bool startUpgrade = !ex->info()->isCurrentVersion(ver) || forceUpdate;
            if (startUpgrade) {
                LOG(INFO) << "Upgrading [" << name << "] from [" 
                          << ex->info()->versionString() 
                          << "] to [" << ver << "]" << (forceUpdate ? " (FORCED)" : "");
                QString baseUrl = extensionobj["base"].toString();
                QStringList filesList = extensionobj["files"].toString().split(',');
                for (QString filename : filesList) {
                    QString targetDir = filesystem::buildPath(QStringList() 
                                                              << qApp->applicationDirPath() << "extensions");
                    QString tempFilename = targetDir + filename + QString(kLocalFilesSuffix);
                    LOG(INFO) << "Downloading extension file [" 
                              << filename << "] from [" + baseUrl + "] to [" << targetDir << "]";
                    result = result && downloadFile(baseUrl + filename, tempFilename);
                    if (!result || !ExtensionLoader::verifyExtension(tempFilename)) {
                        LOG(WARNING) << "Removing [" << tempFilename << "]";
                        QFile file(tempFilename);
                        if (file.open(QIODevice::ReadWrite)) {
                            file.remove();
                            file.close();
                        }
                    } else {
                        m_downloadedFiles.push_back(tempFilename);
                    }
                }
            } else {
                LOG(INFO) << "Extension [" << name << "] is up to date!";
            }
        }
    }
    return result;
}

void UpdateManager::performAsyncUpdate()
{
    _TRACE;
    if (m_future.isRunning()) {
        // Already updating!
        return;
    }
    m_future = QtConcurrent::run(this, &UpdateManager::update);
    m_futureWatcher.setFuture(m_future);
}

void UpdateManager::downloadProgressUpdated(qint64 bytesReceived, qint64 bytesTotal)
{
    if (sender() != nullptr && static_cast<QNetworkReply*>(sender())->error() == QNetworkReply::NoError) {
        float percentage = (static_cast<float>(bytesReceived) / static_cast<float>(bytesTotal)) * 100;
        VLOG(9) << "Download progress [" << bytesReceived << "/" << bytesTotal << "] (" 
                << percentage << "%)";
    }
    
}
bool UpdateManager::performedUpdate() const
{
    return m_performedUpdate;
}
