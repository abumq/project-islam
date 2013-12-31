#include "core/download_manager.h"
#include <QFile>
#include <QEventLoop>
#include <QNetworkReply>
#include "core/logging/logging.h"
#include "core/utils/memory.h"

DownloadManager::DownloadManager(QObject* parent) :
    QNetworkAccessManager(parent)
{
    _TRACE;
}

DownloadManager::~DownloadManager()
{
    _TRACE;
    for (QEventLoop* loop : m_downloadingEventLoops) {
        loop->exit(1);
        m_downloadingEventLoops.removeOne(loop);
    }
}

bool DownloadManager::downloadFile(const QString& url, const QString& filename)
{
    _TRACE;
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
    return returnVal;
}

QByteArray DownloadManager::downloadBytes(const QString& url, bool* ok)
{
    _TRACE;
    QNetworkReply *networkReply = get(QNetworkRequest(url));
    QEventLoop loop;
    QObject::connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(networkReply, SIGNAL(downloadProgress(qint64,qint64)), 
                     this, SIGNAL(downloadProgress(qint64,qint64)));
    m_downloadingEventLoops.push_back(&loop);
    loop.exec();
    if (m_downloadingEventLoops.contains(&loop)) {
        m_downloadingEventLoops.removeOne(&loop);
    }
    if (networkReply->error() == QNetworkReply::NoError) {
        if (ok != nullptr) {
            *ok = true;
        }
        return networkReply->readAll();
    }
    LOG(ERROR) << "Network error occured while downloading bytes from URL [" <<
                  url << "], error [" << networkReply->errorString() << "]";
    if (ok != nullptr) {
        *ok = false;
    }
    return QByteArray();
}
