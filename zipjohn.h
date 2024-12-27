#ifndef ZIPJOHN_H
#define ZIPJOHN_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <zlib.h>  // If you're using zlib for decompression (not required for miniz)
#include "miniz.h"  // This might be replaced with your preferred zip library.

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
