#ifndef ZIPCRACKER_H
#define ZIPCRACKER_H

#include <string>
#include <vector>

// Function to load the password list from a file
std::vector<std::string> loadPasswordList(const std::string &filename);

// Function to check if a given password works on the ZIP file
bool checkPassword(const std::string &zipFilePath, const std::string &password);

#endif // ZIPCRACKER_H
