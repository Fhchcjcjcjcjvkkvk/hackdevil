#include "zipjoh.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: zipjohn <zipfile> <passwordlist>" << std::endl;
        return 1;
    }

    std::string zipFile = argv[1];
    std::string passwordListFile = argv[2];

    ZipCracker cracker(zipFile, passwordListFile);
    cracker.crackZip();

    return 0;
}
