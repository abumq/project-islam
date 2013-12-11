#include "core/update/update_manager.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QEventLoop>
#include <QtConcurrent/QtConcurrent>
#include <QJsonValue>
#include "core/logging.h"
#include "core/memory.h"
#include "core/version.h"
#include "core/settings_loader.h"

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
    
    // Update timer
    m_updateTimer.setInterval(kCheckIntervalInMs);
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(performAsyncUpdate()));
    // Single shot anyway!
    performAsyncUpdate();
    m_updateTimer.start(kCheckIntervalInMs);
}

UpdateManager::~UpdateManager()
{
    if (m_future.isRunning()) {
        _LOG(WARNING) << "Updater was running while killed process.";
        m_future.cancel();
    }
    memory::deleteAll(m_networkManager);
}

bool UpdateManager::downloadFile(const QString& url, const QString& downloadPath)
{
    QNetworkReply *networkReply = m_networkManager->get(QNetworkRequest(url));
    QEventLoop loop;
    QObject::connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    
    QUrl aUrl(url);
    QFileInfo fileInfo=aUrl.path();
    
    bool returnVal;
    QFile file(downloadPath + QString(QDir::separator()) + fileInfo.fileName());
    if (!file.open(QIODevice::WriteOnly)) {
        returnVal = false;
    } else {
        file.write(downloadBytes(url, &returnVal));
        file.close();
        returnVal = true;
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

bool UpdateManager::synchronousUpdate()
{
    if (!needToCheckForUpdates()) {
        _LOG(DEBUG) << "Ignorning updater";
        return true;
    }
    bool result = false;
    _LOG(INFO) << "Checking for updates...";
    
    bool downloadOk = false;
    QString vinfoContents = QString(downloadBytes(versionInfoUrl(), &downloadOk));
    if (downloadOk) {
        _LOG(DEBUG) << "VInfo downloaded from server: " << std::endl << vinfoContents;
        // Deserialize json
    }
    
    if (result) {
        m_lastChecked = QDate::currentDate();
        SettingsLoader().saveSettings("update_checked", QVariant(m_lastChecked.toString()));
    }
    return result;
}

void UpdateManager::performAsyncUpdate()
{
    if (m_future.isRunning()) {
        // Already updating!
        return;
    }
    m_future = QtConcurrent::run(this, &UpdateManager::synchronousUpdate);
    m_futureWatcher.setFuture(m_future);
}
