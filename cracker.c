#include "cracker.h"

int try_password(const char *zip_filename, const char *password) {
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));

    // Open the zip file
    if (!mz_zip_reader_init_file(&zip, zip_filename, 0)) {
        return 0;
    }

    // Try to open the first file in the ZIP using the password
    if (!mz_zip_reader_is_password_protected(&zip)) {
        mz_zip_reader_end(&zip);
        return 0;
    }

    if (mz_zip_reader_set_password(&zip, password)) {
        // Try to extract the first file
        if (mz_zip_reader_extract_to_callback(&zip, 0, NULL, 0)) {
            mz_zip_reader_end(&zip);
            return 1;
        }
    }

    mz_zip_reader_end(&zip);
    return 0;
}

void print_key_found(const char *password) {
    printf("KEY FOUND: [%s]\n", password);
}

void print_key_not_found() {
    printf("KEY NOT FOUND\n");
}
