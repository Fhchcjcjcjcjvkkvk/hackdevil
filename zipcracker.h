// zipcracker.h
#ifndef ZIPCRACKER_H
#define ZIPCRACKER_H

#include <string>
#include <vector>

// Forward declaration of functions
bool testPassword(const std::string& zipFile, const std::string& password);
std::vector<std::string> loadPasswordList(const std::string& passwordListFile);

#endif // ZIPCRACKER_H
