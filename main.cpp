// main.cpp
#include <iostream>
#include <vector>
#include "zipcracker.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: zipcracker <zipfile> -P <passwordlist>" << std::endl;
        return 1;
    }

    std::string zip_filename = argv[1];
    std::string password_list_file = argv[3];
    std::vector<std::string> password_list;

    if (!load_password_list(password_list_file, password_list)) {
        std::cerr << "Failed to load password list!" << std::endl;
        return 1;
    }

    bool found = false;
    for (const auto& password : password_list) {
        if (test_password(zip_filename, password)) {
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "KEY NOT FOUND" << std::endl;
    }

    return 0;
}
