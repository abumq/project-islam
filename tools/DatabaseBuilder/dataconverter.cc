#include "dataconverter.h"

#include <easylogging++.h>
#include <string>
#include <fstream>
#include <algorithm>

DataConverter::DataConverter()
{
}

void DataConverter::startConvert(const std::string& rukuhSajdahFile,
    const std::string& tableName, const std::string& dataFilename,
    const std::string& outputFilename)
{
    std::ifstream rslFile;
    rslFile.open(rukuhSajdahFile.c_str(), std::ifstream::in);

    std::ifstream dataFile;
    dataFile.open(dataFilename.c_str(), std::ifstream::in);

    std::ofstream outputFile;
    outputFile.open(outputFilename.c_str(), std::ofstream::out);

    outputFile << " -- " << tableName << " table \n\
 --     ID \n\
 --     QuranChapterID \n\
 --     VerseNumber \n\
 --     VerseText \n\
 --     Rukuh \n\
 --     Sajdah \n\
 DROP TABLE IF EXISTS " << tableName << "; \n\
 CREATE TABLE IF NOT EXISTS " << tableName << " (ID INTEGER PRIMARY KEY ASC, QuranChapterID INTEGER NOT NULL, VerseNumber INTEGER NOT NULL, VerseText STRING NOT NULL, Rukuh BOOLEAN NOT NULL, Sajdah BOOLEAN NOT NULL);" << std::endl;
 
    std::string insertStatement = "INSERT INTO " + tableName + " (QuranChapterID, VerseNumber, VerseText, Rukuh, Sajdah) VALUES (";

    std::vector<std::string> rukuhList;
    std::vector<std::string> sajdahList;
    std::string rslLine;
    bool s = false, r = false;
    while (rslFile.good()) {
        std::getline(rslFile, rslLine);
        if (rslLine == "-rukuh") { s = false; r = true; continue; }
        else if (rslLine == "-sajdah") { r = false; s = true; continue; }
        if (s) sajdahList.push_back(rslLine);
        if (r) rukuhList.push_back(rslLine);
    }

    std::string line;
    while (dataFile.good()) {
        std::getline(dataFile, line);
        if (line.empty()) continue;
        outputFile << insertStatement;
        // Single quotes to double
        std::replace(line.begin(), line.end(), '\'', '\"');
        outputFile << convertLine(line);
        // Rukuh Sajdah check
        if (std::find(rukuhList.begin(), rukuhList.end(), chapVerse(line)) != rukuhList.end()) {
            LOG(INFO) << "Found rukuh in " << line;
            outputFile << "'1', "; 
        } else {
            outputFile << "'0', ";
        }
        if (std::find(sajdahList.begin(), sajdahList.end(), chapVerse(line)) != sajdahList.end()) {
            LOG(INFO) << "Found sajdah in " << line;
           outputFile << "'1');";
        } else {
           outputFile << "'0');";
        }
        outputFile << std::endl;
    }

    rslFile.close();
    dataFile.close();
    outputFile.close();

}
std::vector<std::string> & DataConverter::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::string DataConverter::convertLine(const std::string& line) {
    std::vector<std::string> e;
    std::stringstream ss;
    split(line, '|', e);
    int i = 1;
    for (std::string row : e) {
        switch (i++) {
        case 1:
           ss << row << ", ";
           break;
        case 2:
           ss << row << ", ";
           break;
        case 3:  
           ss << "'" << row << "', ";
           break;
           i = 1;
        }
    }
    return ss.str();
}

std::string DataConverter::chapVerse(const std::string& line) {
    std::vector<std::string> e;
    std::stringstream ss;
    split(line, '|', e);
    int i = 1;
    for (std::string row : e) {
        switch (i++) {
        case 1:
           ss << row << "|";
           break;
        case 2:
           ss << row;
           break;
        case 3:
           i = 1;
        }
    }
    return ss.str();
}
