#ifndef DATABASE_HH
#define DATABASE_HH

#include <QList>
#include <QDir>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QVariantMap>

namespace db {
    typedef QList<QSqlRecord> QueryResult;
    
    static const QString kSqlCommentBegin = "--";
    static const QString kDefaultDatabaseName = QDir::home().path() + QDir::separator() + ".projislam.db";
    
class DatabaseManager {
public:
    static const QString kDefaultDatabaseName;
    explicit DatabaseManager(const QString& uniqueId = "DefaultDataManager");
    virtual ~DatabaseManager(void);
    const db::QueryResult& query(const QString& query, const QVariantMap& arguments = QVariantMap());
    void reset(void);
    const db::QueryResult& lastQueryResult(void) const;
    const QString& lastQuery(void) const;
    bool lastQuerySuccessful(void) const;
    
private:
    QString m_uniqueId;
    QSqlDatabase m_sqlDatabase;
    QString m_lastQuery;
    db::QueryResult m_lastQueryResult;
    bool m_lastQuerySuccessful;
    int m_connections;
};

} // namespace db
#endif // DATABASE_HH
