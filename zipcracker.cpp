// zipcracker.cpp
#include "zipcracker.h"
#include <iostream>
#include <fstream>
#include <miniz.h>

bool crack_zip_password(const std::string& zip_filename, const std::string& password) {
    // This function uses miniz to test the password.
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    if (!mz_zip_reader_init_file(&zip_archive, zip_filename.c_str(), 0)) {
        return false;
    }

    if (mz_zip_reader_is_password_required(&zip_archive)) {
        if (!mz_zip_reader_set_password(&zip_archive, password.c_str())) {
            mz_zip_reader_end(&zip_archive);
            return false;
        }
    }

    // Try to extract the first file to check the password
    if (mz_zip_reader_extract_to_callback(&zip_archive, 0, nullptr, nullptr, nullptr)) {
        mz_zip_reader_end(&zip_archive);
        return true; // Password is correct
    }

    mz_zip_reader_end(&zip_archive);
    return false; // Password is incorrect
}

bool load_password_list(const std::string& password_list_file, std::vector<std::string>& password_list) {
    std::ifstream file(password_list_file);
    std::string line;
    if (!file.is_open()) {
        return false;
    }

    while (std::getline(file, line)) {
        password_list.push_back(line);
    }
    return true;
}

bool test_password(const std::string& zip_filename, const std::string& password) {
    // Try testing the password
    if (crack_zip_password(zip_filename, password)) {
        std::cout << "KEY FOUND: " << password << std::endl;
        return true;
    }
    return false;
}
