#include <iostream>
#include <vector>
#include <string>
#include "zipcracker.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: zipcracker <zipfile> <passwordlist>" << std::endl;
        return 1;
    }

    std::string zipFilePath = argv[1];
    std::string passwordListFile = argv[2];

    std::vector<std::string> passwords = loadPasswordList(passwordListFile);

    if (passwords.empty()) {
        std::cerr << "No passwords to test!" << std::endl;
        return 1;
    }

    for (const std::string &password : passwords) {
        if (checkPassword(zipFilePath, password)) {
            std::cout << "KEY FOUND: [" << password << "]" << std::endl;
            return 0;
        }
    }

    std::cout << "KEY NOT FOUND" << std::endl;
    return 0;
}
