#ifndef DATABASE_BUILDER_HH
#define DATABASE_BUILDER_HH

#include <QString>

namespace db {

class DatabaseBuilder {
public:
    static bool build(const QString& sqlFile, bool stopOnFirstError = true);
private:
    DatabaseBuilder(void);
    DatabaseBuilder(const DatabaseBuilder&);
    DatabaseBuilder& operator=(const DatabaseBuilder&);
};

} // namespace db

#endif // DATABASE_BUILDER_HH
