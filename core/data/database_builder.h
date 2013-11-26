#ifndef DATABASE_BUILDER_HH
#define DATABASE_BUILDER_HH

#include <QString>

namespace data {

class DatabaseBuilder {
public:
    enum class DataType {
        QuranChapters, QuranArabic, QuranTransliteration, QuranTranslation, Dua
    };

    static bool build(const DataType& type,  bool stopOnFirstError = true);
    static QString dataTypeToFilename(const DataType& type);
private:
    DatabaseBuilder(void);
    DatabaseBuilder(const DatabaseBuilder&);
    DatabaseBuilder& operator=(const DatabaseBuilder&);
    
    static bool build(const QString& sqlFile, bool stopOnFirstError = true);
};

} // namespace data

#endif // DATABASE_BUILDER_HH
