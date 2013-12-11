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
    static const int kCheckIntervalInMs = 100;
public:
    explicit UpdateManager(QObject* parent = 0);
    virtual ~UpdateManager();
protected:
    bool downloadFile(const QString&, const QString&);
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
};

#endif // UPDATE_MANAGER_H
