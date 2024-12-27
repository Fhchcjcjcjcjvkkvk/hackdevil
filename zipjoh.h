#ifndef ZIPJOHN_H
#define ZIPJOHN_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "miniz.h"  // Using miniz for ZIP file handling.

class ZipCracker {
public:
    ZipCracker(const std::string& zipFile, const std::string& passwordListFile);
    bool crackZip();
    void printResult(bool found, const std::string& password = "");

private:
    bool testPassword(const std::string& password);
    std::string zipFileName;
    std::string passwordListFileName;
    std::vector<std::string> passwordList;
    mz_zip_archive zipArchive;
};

#endif // ZIPJOHN_H
