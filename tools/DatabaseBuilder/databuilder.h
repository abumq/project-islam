#ifndef DATABUILDER_H
#define DATABUILDER_H

#include <string>
#include <QString>
#include <QVariantMap>
#include <QSqlDatabase>
#include <QList>

class DataBuilder
{
public:
    DataBuilder(const std::string& sqlFilename, 
                const std::string& sqliteFilename);
    ~DataBuilder();
    bool build();
private:
    QString m_databaseFile;
    QString m_sqlFile;
    QSqlDatabase m_sqlDatabase;
    QList<QSqlRecord> m_lastQueryResult;
    bool m_lastQuerySuccessful;
    int m_connections;
    const QList<QSqlRecord>& query(const QString& query, 
                                   const QVariantMap& arguments = QVariantMap());
};

#endif // DATABUILDER_H
