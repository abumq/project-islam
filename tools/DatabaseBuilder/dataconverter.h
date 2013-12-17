#ifndef DATACONVERTER_H
#define DATACONVERTER_H

#include <string>
#include <vector>

class ConvertData {
public:
    ConvertData(const std::string& baseInfoFilename, const std::string& tableName, 
                const std::string& dataFilename, const std::string& outputFilename) :
        baseInfoFilename(baseInfoFilename),
        tableName(tableName),
        dataFilename(dataFilename),
        outputFilename(outputFilename) {
        
    }
    
    bool operator==(const std::string& tableName) {
        return this->tableName == tableName;
    }

    std::string baseInfoFilename;
    std::string tableName;
    std::string dataFilename;
    std::string outputFilename;
};

class DataConverter
{
public:
    DataConverter();
    
    void startConvert(ConvertData*);
private:
    std::vector<std::string> &split(const std::string &s, char delim, 
                                    std::vector<std::string> &elems);
    std::string convertLine(const std::string& line, std::vector<std::string>* sajdahList, 
                            std::vector<std::string>* rukuhList,
                            std::vector<std::string>* manzilList,
                            std::vector<std::string>* hizbQuarterList);
    std::string chapVerse(const std::string& line);
};

#endif // DATACONVERTER_H
