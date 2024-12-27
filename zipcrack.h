#ifndef ZIPCRACKER_H
#define ZIPCRACKER_H

#include <string>
#include <vector>
#include <zip.h>

class ZipCracker {
public:
    ZipCracker(const std::string &zipFilePath);
    bool crackPassword(const std::vector<std::string> &passwordList);

private:
    bool tryPassword(const std::string &password);
    std::string zipFilePath;
};

#endif
