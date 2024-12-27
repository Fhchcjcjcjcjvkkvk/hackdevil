#include "zipcracker.h"
#include <iostream>
#include <fstream>
#include <miniz.h>  // Include miniz library for handling ZIP files

// Function to load the password list from a file
std::vector<std::string> loadPasswordList(const std::string &filename) {
    std::vector<std::string> passwords;
    std::ifstream file(filename);
    std::string password;

    if (!file.is_open()) {
        std::cerr << "Error opening password list file." << std::endl;
        return passwords;
    }

    while (std::getline(file, password)) {
        passwords.push_back(password);
    }

    file.close();
    return passwords;
}

// Function to check if a given password works on the ZIP file
bool checkPassword(const std::string &zipFilePath, const std::string &password) {
    // Open the ZIP file with miniz
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));

    // Try to open the ZIP file
    if (!mz_zip_reader_init_file(&zip, zipFilePath.c_str(), 0)) {
        return false;
    }

    // Loop through all the files in the ZIP archive
    int fileCount = mz_zip_reader_get_num_files(&zip);
    for (int i = 0; i < fileCount; ++i) {
        mz_zip_archive_file_stat fileStat;
        if (!mz_zip_reader_file_stat(&zip, i, &fileStat)) {
            mz_zip_reader_end(&zip);
            return false;
        }

        // Attempt to extract the file with the given password
        if (mz_zip_reader_extract_to_memory(&zip, i, nullptr, 0, 0)) {
            mz_zip_reader_end(&zip);
            return true;  // Password is correct
        }
    }

    mz_zip_reader_end(&zip);
    return false;  // Password is incorrect
}
