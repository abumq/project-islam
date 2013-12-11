#include "core/update/update_manager.h"

#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QEventLoop>
#include <QtConcurrent/QtConcurrent>
#include <QJsonDocument>

#include "core/logging.h"
#include "core/memory.h"
#include "core/version.h"
#include "core/settings_loader.h"
#include "core/extension/abstract_extension.h"
#include "core/extension/extension_info.h"
#include "core/extension/extension_bar.h"

#define UPDATE_MANAGER_LOGGER_ID "update_manager"
#undef _LOG
#define _LOG(LEVEL) CLOG(LEVEL, UPDATE_MANAGER_LOGGER_ID)

const char* UpdateManager::kServerUrlBase = "http://www.icplusplus.com";
const char* UpdateManager::kVersionInfoFilename = "tools/project-islam/vinfo.txt";

UpdateManager::UpdateManager(QObject *parent) :
    m_networkManager(new QNetworkAccessManager(parent))
{
    // Logger
    el::Loggers::getLogger(UPDATE_MANAGER_LOGGER_ID);
    
    // Load m_lastChecked
    const QDate defaultDate = QDate::currentDate().addDays(-1);
    QString lastCheckedStr = 
            SettingsLoader().get("update_checked", 
                                 QVariant(defaultDate.toString())).toString();
    m_lastChecked = QDate::fromString(lastCheckedStr);
    if (m_lastChecked.isNull() || !m_lastChecked.isValid()) {
        m_lastChecked = defaultDate;
    }
    SettingsLoader().saveSettings("update_checked", QVariant(m_lastChecked.toString()));
}

UpdateManager::~UpdateManager()
{
    if (m_future.isRunning()) {
        _LOG(WARNING) << "Updater was running while killed process.";
        m_future.cancel();
    }
    memory::deleteAll(m_networkManager);
}

void UpdateManager::initialize(QApplication* app, ExtensionBar* extensionBar)
{
    m_app = CHECK_NOTNULL(app);
    m_extensionBar = CHECK_NOTNULL(extensionBar);
    // Update timer
    m_updateTimer.setInterval(kCheckIntervalInMs);
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(performAsyncUpdate()));
    // Single shot anyway!
    performAsyncUpdate();
    m_updateTimer.start(kCheckIntervalInMs);
}

bool UpdateManager::downloadFile(const QString& url, const QString& filename)
{
    QNetworkReply *networkReply = m_networkManager->get(QNetworkRequest(url));
    QEventLoop loop;
    QObject::connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    
    bool returnVal;
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        returnVal = false;
    } else {
        file.write(downloadBytes(url, &returnVal));
        file.close();
        if (file.size() == 0) {
            file.remove();
            returnVal = false;
        } else {
            returnVal = true;
        }
    }
    memory::deleteAll(networkReply);
    return returnVal;
}

QByteArray UpdateManager::downloadBytes(const QString& url, bool* ok)
{
    QNetworkReply *networkReply = m_networkManager->get(QNetworkRequest(url));
    QEventLoop loop;
    QObject::connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if (networkReply->error() == QNetworkReply::NoError) {
        if (ok != nullptr) {
            *ok = true;
        }
        return networkReply->readAll();
    }
    _LOG(ERROR) << "Network error occured while downloading bytes from URL [" <<
                   url << "], error [" << networkReply->errorString() << "]";
    if (ok != nullptr) {
        *ok = false;
    }
    return QByteArray();
}

bool UpdateManager::needToCheckForUpdates() const
{
    return m_lastChecked.daysTo(QDate::currentDate()) >= kDaysToCheck;
}

QString UpdateManager::versionInfoUrl() const
{
    return QString(kServerUrlBase) + "/" + QString(kVersionInfoFilename);
}

bool UpdateManager::update()
{
    if (!needToCheckForUpdates()) {
        _LOG(DEBUG) << "Ignorning updater";
        return true;
    }
    TIMED_SCOPE(timer, "Update");
    bool result = false;
    _LOG(INFO) << "Checking for updates...";
    
    bool downloadOk = false;
    QString jsonVInfo = QString(downloadBytes(versionInfoUrl(), &downloadOk));
    if (downloadOk) {
        _LOG(DEBUG) << "VInfo downloaded from server: " << std::endl << jsonVInfo;
        // Deserialize json
        QJsonParseError jsonError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonVInfo.toUtf8(), &jsonError);
        if (jsonError.error == QJsonParseError::NoError) {
            QJsonObject jsonObject = jsonDocument.object();
            QString currOs;
#if defined(Q_OS_LINUX)
            currOs = "linux";
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
            currOs = "win";
#elif defined(Q_OS_MAC)
            currOs = "mac";
#else
            _LOG(ERROR) << "Operating system not supported for update";
            return false;
#endif // defined(Q_OS_LINUX)
            QJsonObject osObj = jsonObject[currOs].toObject();
            if (osObj["available"].toBool()) {
                result = updatePlatform(&osObj) && updateExtensions(&osObj);
            } else {
                _LOG(ERROR) << "No downloadable update available for [" 
                            << currOs << "]";
                
            }
            
        } else {
            _LOG(ERROR) << "Error while parsing json. Error [" 
                        << jsonError.errorString() << "]";
        }
    }
    if (result) {
        m_lastChecked = QDate::currentDate();
        SettingsLoader().saveSettings("update_checked", QVariant(m_lastChecked.toString()));
    }
    return result;
}

bool UpdateManager::updatePlatform(QJsonObject* jsonObject)
{    
    QJsonObject platformObj = (*jsonObject)["platform"].toObject();
    QString ver = platformObj["version"].toString();
    bool forceUpdate = platformObj["force_update"].toBool();
    bool startUpgrade = !version::isCurrentVersion(ver) || forceUpdate;
    bool result = true; // We will mark it false if download fail
    if (startUpgrade) {
        _LOG(INFO) << "Upgrading platform from [" << version::versionString() 
                   << "] to [" << ver << "]" << (forceUpdate ? " (FORCED)" : "");
        QString baseUrl = platformObj["base"].toString();
        QStringList filesList = platformObj["files"].toString().split(',');
        for (QString filename : filesList) {
            _LOG(INFO) << "Downloading platform file [" 
                       << filename << "] from [" + baseUrl + "]";
            QString targetDir = m_app->applicationDirPath() + "/";
            QString tempFilename = targetDir + filename + ".updated";
            result = result && downloadFile(baseUrl + filename, tempFilename);
        }
    }
    return result;
}

bool UpdateManager::updateExtensions(QJsonObject* jsonObject)
{
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
                _LOG(INFO) << "Upgrading [" << name << "] from [" 
                           << ex->info()->versionString() 
                           << "] to [" << ver << "]" << (forceUpdate ? " (FORCED)" : "");
                QString baseUrl = extensionobj["base"].toString();
                QStringList filesList = extensionobj["files"].toString().split(',');
                for (QString filename : filesList) {
                    _LOG(INFO) << "Downloading extension file [" 
                               << filename << "] from [" + baseUrl + "]";
                    QString targetDir = m_app->applicationDirPath() + "/extensions/";
                    QString tempFilename = targetDir + filename + ".updated";
                    result = result && downloadFile(baseUrl + filename, tempFilename);
                }
            }
        }
    }
    return result;
}

void UpdateManager::performAsyncUpdate()
{
    if (m_future.isRunning()) {
        // Already updating!
        return;
    }
    m_future = QtConcurrent::run(this, &UpdateManager::update);
    m_futureWatcher.setFuture(m_future);
}
