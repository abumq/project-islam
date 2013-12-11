#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include <QObject>
#include <QDate>
#include <QTimer>
#include <QFuture>
#include <QFutureWatcher>
class QNetworkAccessManager;

class UpdateManager : public QObject
{
    Q_OBJECT
    static const qint64 kDaysToCheck = 1;
    static const qint64 kCheckIntervalInMs = 7200000; // Every 3 hours
    static const char* kServerUrlBase;
    static const char* kVersionInfoFilename;
public:
    explicit UpdateManager(QObject* parent = 0);
    virtual ~UpdateManager();
protected:
    bool downloadFile(const QString& url, const QString& path);
    QByteArray downloadBytes(const QString& url, bool* ok = nullptr);
private slots:
    bool synchronousUpdate();
    void performAsyncUpdate();
private:
    QNetworkAccessManager* m_networkManager;
    QDate m_lastChecked;
    QTimer m_updateTimer;
    QFuture<void> m_future;
    QFutureWatcher<void> m_futureWatcher;
    
    bool needToCheckForUpdates() const;
    QString versionInfoUrl() const;
};

#endif // UPDATE_MANAGER_H
