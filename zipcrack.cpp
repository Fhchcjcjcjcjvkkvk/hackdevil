#include "zipcrack.h"
#include <iostream>
#include <fstream>
#include <vector>

ZipCracker::ZipCracker(const std::string &zipFilePath)
    : zipFilePath(zipFilePath) {}

bool ZipCracker::tryPassword(const std::string &password) {
    // Open the zip file
    zip_t *zip = zip_open(zipFilePath.c_str(), ZIP_RDONLY, nullptr);
    if (!zip) {
        std::cerr << "Failed to open zip file!" << std::endl;
        return false;
    }

    // Try to unzip with the password
    if (zip_file_set_encryption(zip, password.c_str()) == 0) {
        zip_close(zip);
        return true; // Password found
    }

    zip_close(zip);
    return false; // Password not correct
}

bool ZipCracker::crackPassword(const std::vector<std::string> &passwordList) {
    for (const auto &password : passwordList) {
        if (tryPassword(password)) {
            std::cout << "KEY FOUND: " << password << std::endl;
            return true;
        }
    }
    std::cout << "KEY NOT FOUND" << std::endl;
    return false;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: pdfjohn.exe <zip_file> <password_list_file>" << std::endl;
        return 1;
    }

    std::string zipFilePath = argv[1];
    std::string passwordListFilePath = argv[2];

    // Load password list
    std::ifstream passwordListFile(passwordListFilePath);
    if (!passwordListFile.is_open()) {
        std::cerr << "Failed to open password list file!" << std::endl;
        return 1;
    }

    std::vector<std::string> passwordList;
    std::string password;
    while (std::getline(passwordListFile, password)) {
        passwordList.push_back(password);
    }

    ZipCracker cracker(zipFilePath);
    cracker.crackPassword(passwordList);

    return 0;
}
