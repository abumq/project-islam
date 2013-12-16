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
            CREATE TABLE IF NOT EXISTS " << tableName << " (ID INTEGER PRIMARY KEY ASC, QuranChapterID INTEGER NOT NULL, VerseNumber INTEGER NOT NULL, Rukuh BOOLEAN NOT NULL, Sajdah BOOLEAN NOT NULL, Manzil BOOLEAN NOT NULL, HizbQuarter BOOLEAN NOT NULL, VerseText STRING NOT NULL);" << std::endl;
    
    std::string insertStatement = "INSERT INTO " + tableName + " (QuranChapterID, VerseNumber, Rukuh, Sajdah, Manzil, HizbQuarter, VerseText) VALUES (";
    
    std::vector<std::string> rukuhList;
    std::vector<std::string> sajdahList;
    std::vector<std::string> manzilList;
    std::vector<std::string> hizbQuarterList;
    std::string rslLine;
    bool s = false, r = false, m = false, hq = false; // Sajdah, Rukuh, Manzil, Hizb-Quarter, Juz
    while (rslFile.good()) {
        std::getline(rslFile, rslLine);
        if (rslLine.empty() || rslLine[0] == '#') continue; // Comment!
        if (rslLine == "-rukuh") { s = false; m = false; hq = false; r = true; continue; }
        else if (rslLine == "-sajdah") { r = false; m = false; hq = false; s = true; continue; }
        else if (rslLine == "-manzil") { r = false; m = true; hq = false; s = false; continue; }
        else if (rslLine == "-hizb-quarter") { r = false; m = false; hq = true; s = false; continue; }
        if (s) sajdahList.push_back(rslLine);
        if (r) rukuhList.push_back(rslLine);
        if (m) manzilList.push_back(rslLine);
        if (hq) hizbQuarterList.push_back(rslLine);
    }
    
    std::string line;
    while (dataFile.good()) {
        std::getline(dataFile, line);
        if (line.empty()) continue;
        outputFile << insertStatement;
        // Single quotes to double
        std::replace(line.begin(), line.end(), '\'', '\"');
        outputFile << convertLine(line, &sajdahList, &rukuhList, &manzilList, &hizbQuarterList);
        outputFile << ");";
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

std::string DataConverter::convertLine(const std::string& line, 
                                       std::vector<std::string>* sajdahList,
                                       std::vector<std::string>* rukuhList,
                                       std::vector<std::string>* manzilList,
                                       std::vector<std::string>* hizbQuarterList) {
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
            // Rukuh Sajdah check
            if (std::find(rukuhList->begin(), rukuhList->end(), chapVerse(line)) != rukuhList->end()) {
                LOG(INFO) << "Found rukuh in " << line;
                ss << "'1', "; 
            } else {
                ss << "'0', ";
            }
            if (std::find(sajdahList->begin(), sajdahList->end(), chapVerse(line)) != sajdahList->end()) {
                LOG(INFO) << "Found sajdah in " << line;
                ss << "'1', ";
            } else {
                ss << "'0', ";
            }
            if (std::find(manzilList->begin(), manzilList->end(), chapVerse(line)) != manzilList->end()) {
                LOG(INFO) << "Found manzil in " << line;
                ss << "'1', ";
            } else {
                ss << "'0', ";
            }
            if (std::find(hizbQuarterList->begin(), hizbQuarterList->end(), chapVerse(line)) != hizbQuarterList->end()) {
                LOG(INFO) << "Found hizb-quarter in " << line;
                ss << "'1', ";
            } else {
                ss << "'0', ";
            }
            break;
        case 3:  
            ss << "'" << row << "'";
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
