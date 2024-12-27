// zipcracker.cpp
#include "zipcracker.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <miniz.h>  // Include miniz library for ZIP file handling

bool testPassword(const std::string& zipFile, const std::string& password) {
    mz_zip_archive zipArchive;
    memset(&zipArchive, 0, sizeof(zipArchive));

    // Open the ZIP file
    if (!mz_zip_reader_init_file(&zipArchive, zipFile.c_str(), 0)) {
        std::cerr << "Failed to open ZIP file!" << std::endl;
        return false;
    }

    // Try to decrypt using the password
    if (mz_zip_reader_is_password_set(&zipArchive)) {
        if (!mz_zip_reader_set_password(&zipArchive, password.c_str())) {
            mz_zip_reader_end(&zipArchive);
            return false;
        }
    }

    // Test extracting a file (we don't care about the actual content, just if it's correct)
    if (mz_zip_reader_extract_to_file(&zipArchive, 0, "temp.txt")) {
        mz_zip_reader_end(&zipArchive);
        return true;
    }

    mz_zip_reader_end(&zipArchive);
    return false;
}

std::vector<std::string> loadPasswordList(const std::string& passwordListFile) {
    std::vector<std::string> passwords;
    std::ifstream file(passwordListFile);
    std::string line;

    while (std::getline(file, line)) {
        passwords.push_back(line);
    }

    return passwords;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: zipcracker <zip-file> <password-list-file>" << std::endl;
        return -1;
    }

    std::string zipFile = argv[1];
    std::string passwordListFile = argv[2];

    // Load passwords from the file
    std::vector<std::string> passwords = loadPasswordList(passwordListFile);

    // Try each password in the list
    for (const std::string& password : passwords) {
        if (testPassword(zipFile, password)) {
            std::cout << "KEY FOUND: [" << password << "]" << std::endl;
            return 0;
        }
    }

    std::cout << "KEY NOT FOUND" << std::endl;
    return 0;
}
