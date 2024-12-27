#include "zipjohn.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <miniz.h>  // Include the miniz library for ZIP handling.

ZipCracker::ZipCracker(const std::string& zipFile, const std::string& passwordListFile)
    : zipFileName(zipFile), passwordListFileName(passwordListFile) {}

bool ZipCracker::testPassword(const std::string& password) {
    // Open the ZIP archive using the miniz library
    if (!mz_zip_reader_init_file(&zipArchive, zipFileName.c_str(), 0)) {
        return false; // Failed to open ZIP file
    }

    // Try to extract with the given password
    if (!mz_zip_reader_set_password(&zipArchive, password.c_str())) {
        mz_zip_reader_end(&zipArchive);
        return false;  // Password incorrect
    }

    // Try to extract one file (can be adjusted to your needs)
    if (mz_zip_reader_extract_to_file(&zipArchive, 0, "output.txt", 0)) {
        mz_zip_reader_end(&zipArchive);
        return true;  // Password works, ZIP file is cracked
    }

    mz_zip_reader_end(&zipArchive);
    return false;
}

bool ZipCracker::crackZip() {
    // Load the password list
    std::ifstream infile(passwordListFileName);
    std::string password;

    if (!infile) {
        std::cerr << "Password list file not found!" << std::endl;
        return false;
    }

    // Iterate over each password in the list
    while (std::getline(infile, password)) {
        password = password.substr(0, password.find('\r')); // Remove carriage returns if any
        bool found = testPassword(password);

        if (found) {
            printResult(true, password);
            return true;  // Password found
        }
    }

    printResult(false);
    return false;  // No password found
}

void ZipCracker::printResult(bool found, const std::string& password) {
    if (found) {
        std::cout << "KEY FOUND: " << password << std::endl;
    } else {
        std::cout << "KEY NOT FOUND" << std::endl;
    }
}
