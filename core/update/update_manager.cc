#include "core/update/update_manager.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QEventLoop>
#include <QtConcurrent/QtConcurrent>
#include "core/logging.h"
#include "core/memory.h"
#include "core/settings_loader.h"

UpdateManager::UpdateManager(QObject *parent) :
    m_networkManager(new QNetworkAccessManager(parent))
{
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
    m_updateTimer.start(kCheckIntervalInMs);
}

UpdateManager::~UpdateManager()
{
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
        file.write(networkReply->readAll());
        file.close();
        returnVal = true;
    }
    memory::deleteAll(networkReply);
    return returnVal;
}

bool UpdateManager::needToCheckForUpdates() const
{
    return m_lastChecked.daysTo(QDate::currentDate()) >= kDaysToCheck;
}

bool UpdateManager::synchronousUpdate()
{
    if (!needToCheckForUpdates()) {
        LOG(DEBUG) << "Ignorning updater";
        return true;
    }
    bool result = false;
    LOG(INFO) << "Checking for updates...";
    
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
