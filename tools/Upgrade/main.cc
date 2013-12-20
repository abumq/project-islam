#include <QCoreApplication>
#include <QFile>
#include <QStringList>
#include <QProcess>
//#include <easylogging++.h>

//_INITIALIZE_EASYLOGGINGPP

void performUpdate() {
    QFile f("upgrade.info");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    while (!f.atEnd()) {
        QString filename = f.readLine();
        if (filename.isEmpty()) {
            continue;
        }
        //LOG(INFO) << "Filename: " << filename;
        /*QString downloadedFilename = filesystem::buildFilename(
                        QStringList() << qApp->applicationDirPath() << filename + QString(kLocalFilesSuffix));
            const int kExtraSuffix = (QString(kRemoteFilesSuffix) + QString(kLocalFilesSuffix)).length();
            QString currFilename = downloadedFilename.mid(
                        0,  downloadedFilename.length() - kExtraSuffix);
                        
            QFile currFile(currFilename);
            QFile::Permissions perms;
            if (currFile.exists()) {
                perms = currFile.permissions();
                currFile.rename(currFilename + ".old");
                currFile.close();
            }
            QFile downloadedFile(downloadedFilename);
            result = downloadedFile.rename(currFilename);
            result = downloadedFile.setPermissions(perms) && result;
            if (result && currFile.exists()) {
                currFile.remove();
            }
            LOG(INFO) << "Platform has been successfully updated!";*/
    }
    f.close();
    f.remove();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "false");
    
    QStringList projectIslamArgs;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            projectIslamArgs.append(QString(argv[i]));
        }
    }
    performUpdate();
    if (!projectIslamArgs.isEmpty()) {
        QProcess::startDetached(projectIslamArgs[0], projectIslamArgs);
    }
    return 0;
}
