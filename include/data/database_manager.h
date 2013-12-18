#ifndef DATABASE_HH
#define DATABASE_HH

#include <QList>
#include <QDir>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QVariantMap>

#include "settings_loader.h"

namespace data {

    typedef QList<QSqlRecord> QueryResult;
    
    static const QString kSqlCommentBegin = "--";
    static const QString kDefaultDatabaseFilename = "project-islam.db";
class DatabaseManager {
public:
    DatabaseManager(const QString& uniqueId = "DefaultDataManager", const QString& dbFilename = kDefaultDatabaseFilename);
    virtual ~DatabaseManager(void);
    const data::QueryResult& query(const QString& query, const QVariantMap& arguments = QVariantMap());
    void reset(void);
    const data::QueryResult& lastQueryResult(void) const;
    const QString& lastQuery(void) const;
    bool lastQuerySuccessful(void) const;
    
private:
    QString m_uniqueId;
    QSqlDatabase m_sqlDatabase;
    QString m_lastQuery;
    data::QueryResult m_lastQueryResult;
    bool m_lastQuerySuccessful;
    int m_connections;
};

} // namespace data
#endif // DATABASE_HH
