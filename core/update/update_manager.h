#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include <QObject>
#include <QDate>
#include <QTimer>
#include <QFuture>
#include <QFutureWatcher>
class QNetworkAccessManager;
class QJsonObject;
class ExtensionBar;
class QApplication;
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
    void initialize(QApplication* app, ExtensionBar* extensionBar);
protected:
    bool downloadFile(const QString& url, const QString& filename);
    QByteArray downloadBytes(const QString& url, bool* ok = nullptr);
private slots:
    void performAsyncUpdate();
private:
    QNetworkAccessManager* m_networkManager;
    QDate m_lastChecked;
    QTimer m_updateTimer;
    QFuture<void> m_future;
    QFutureWatcher<void> m_futureWatcher;
    QApplication* m_app;
    ExtensionBar* m_extensionBar;
    
    bool needToCheckForUpdates() const;
    QString versionInfoUrl() const;
    
    bool update();
    bool updatePlatform(QJsonObject* jsonObject);
    bool updateExtensions(QJsonObject* jsonObject);
};

#endif // UPDATE_MANAGER_H
