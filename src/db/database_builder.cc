#include "database_builder.hh"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "log/logging.hh"
#include "database_manager.hh"

namespace db {

bool DatabaseBuilder::build(const QString& sqlFile, bool stopOnFirstError)
{
    QFile file(sqlFile);
    CHECK(file.open(QIODevice::ReadOnly)) << "Unable to open SQL file [" << sqlFile << "]";
    
    QTextStream in(&file);
    
    DATABASE_LOG(INFO) << "Reading SQL file...";
    QStringList sqlFromFile;
    while(!in.atEnd()) {
        sqlFromFile.append(in.readLine());
    }
    
    file.close();
    
    DATABASE_LOG(INFO) << "Building database... [" << db::kDefaultDatabaseName << "]";
    db::DatabaseManager manager("DatabaseBuilder");
    foreach (QString sql, sqlFromFile) {
        if (sql.trimmed().isEmpty() || sql.startsWith(db::kSqlCommentBegin))
            continue;
        CLOG_EVERY_N(100, INFO, "database") << "Current iteration [" << ELPP_COUNTER_POS << "]; SQL: " << sql;
        manager.query(sql);
        if (stopOnFirstError && !manager.lastQuerySuccessful()) {
            return false;
        }
    }
    return true;
}

} // namespace db
