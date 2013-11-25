#include "core/data/database_builder.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "core/logging.h"
#include "core/data/database_manager.h"

namespace data {

bool DatabaseBuilder::build(const QString& sqlFile, bool stopOnFirstError)
{
    QFile file(sqlFile);
    DCHECK(file.open(QIODevice::ReadOnly)) << "Unable to open SQL file [" << sqlFile << "]";
    
    QTextStream in(&file);
    
    DDATA_LOG(INFO) << "Reading SQL file...";
    QStringList sqlFromFile;
    while(!in.atEnd()) {
        sqlFromFile.append(in.readLine());
    }
    
    file.close();
    if (!QDir(data::kDefaultDatabasePath).exists()) {
        DDATA_LOG(INFO) << "Creating path [" << data::kDefaultDatabasePath << "]";
        QDir(data::kDefaultDatabasePath).mkdir(data::kDefaultDatabasePath);
    }
    DDATA_LOG(INFO) << "Building database... [" << data::kDefaultDatabaseName << "]";
    data::DatabaseManager manager("DatabaseBuilder");
    foreach (QString sql, sqlFromFile) {
        if (sql.trimmed().isEmpty() || sql.startsWith(data::kSqlCommentBegin))
            continue;
        CLOG_EVERY_N(100, DEBUG, "data") << "Current iteration [" << ELPP_COUNTER_POS << "]; SQL: " << sql;
        manager.query(sql);
        if (stopOnFirstError && !manager.lastQuerySuccessful()) {
            return false;
        }
    }
    return true;
}

bool DatabaseBuilder::build(const DatabaseBuilder::DataType &type, bool stopOnFirstError)
{
    return build(dataTypeToFilename(type), stopOnFirstError);
}

QString DatabaseBuilder::dataTypeToFilename(const DatabaseBuilder::DataType &type)
{
    if (type == DataType::QuranChapters) {
        return ":/quran/quran_chapters";
    }
    if (type == DataType::QuranArabic) {
        return ":/quran/quran_arabic";
    }
    if (type == DataType::QuranEnglish) {
        return ":/quran/quran_english";
    }
    if (type == DataType::Dua) {
        return ":dua";
    }
    return QString();
}

} // namespace database
