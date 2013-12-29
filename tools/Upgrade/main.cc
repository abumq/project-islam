#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QProcess>
#include "easylogging++.h"

_INITIALIZE_EASYLOGGINGPP

bool performUpdate(const QString&upgradeFile);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (argc == 0) {
        LOG(ERROR) << "Invalid number of args, [upgrade-file] and [app] required";
        return 1;
    }
    int upgradeFileIdx = qApp->arguments().indexOf("--upgrade-file");
    if (upgradeFileIdx == -1) {
        LOG(ERROR) << "Upgrade file not found!";
        return 1;
    }
    QString upgradeFile = qApp->arguments().at(upgradeFileIdx + 1);
    QStringList projectIslamArgs;
    int projectIslamArgsStartIdx = qApp->arguments().indexOf("--app");
    if (projectIslamArgsStartIdx != -1) {
        projectIslamArgs.append(qApp->arguments().at(projectIslamArgsStartIdx + 1));
        for (int i = projectIslamArgsStartIdx + 2; i < argc; ++i) {
            projectIslamArgs.append(qApp->arguments().at(i));
        }
    }
    bool result = performUpdate(upgradeFile);
    if (!result && qApp->arguments().contains("--no-launch-on-failure")) {
        return 1;
    }
    if (!projectIslamArgs.isEmpty()) {
        QProcess::startDetached(projectIslamArgs[0], projectIslamArgs);
    }
    return result ? 0 : 1;
}

bool performUpdate(const QString& upgradeFile) {
    QFileInfo fi(upgradeFile);
    QFile f(fi.filePath());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LOG(ERROR) << fi.filePath() << ": " << f.errorString();
        return false;
    }
    
    while (!f.atEnd()) {
        QString filename = f.readLine();
        filename = filename.trimmed();
        if (filename.isEmpty() && !filename.endsWith(".zip.upgrade")) {
            continue;
        }
        LOG(INFO) << "Filename: " << filename;
        QFile currFile(filename);
        if (currFile.size() > 0 && currFile.open(QIODevice::ReadWrite)) {
            QString renamedFile = filename.mid(0, filename.indexOf(".zip.upgrade"));
            LOG(INFO) << "Renaming from [" << filename << "] to [" << renamedFile << "]";
            currFile.close();
            // Get permission of existing file
            QFile existingFile(renamedFile);
            QFile::Permissions p;
            if (existingFile.exists()) {
                p = existingFile.permissions();
            } else {
                p = QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner |
                    QFile::ReadGroup | QFile::ReadOther;
            }
            existingFile.remove();
            currFile.rename(renamedFile);
            currFile.setPermissions(p);
        }
    }
    f.close();
    f.remove();
    LOG(INFO) << "Upgrade completed!";
    return true;
}
