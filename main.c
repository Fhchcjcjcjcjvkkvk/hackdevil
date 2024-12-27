#include "mephisto.h"

// Maximum length for the password
#define MAX_PASSWORD_LEN 256

// Function to check the password against the zip file
int check_zip_password(const char *zip_file, const char *password) {
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));

    if (!mz_zip_reader_init_file(&zip, zip_file, 0)) {
        return 0; // Failed to initialize the ZIP reader
    }

    if (!mz_zip_reader_is_password_required(&zip)) {
        mz_zip_reader_end(&zip);
        return 0; // ZIP doesn't require a password
    }

    // Attempt to open the first file with the given password
    if (mz_zip_reader_extract_to_mem(&zip, 0, NULL, 0, password, strlen(password)) > 0) {
        mz_zip_reader_end(&zip);
        return 1; // Password is correct
    }

    mz_zip_reader_end(&zip);
    return 0; // Password is incorrect
}

// Function to iterate over the password list
int crack_zip(const char *zip_file, const char *password_list) {
    FILE *file = fopen(password_list, "r");
    if (!file) {
        return 0; // Error opening the password list
    }

    char password[MAX_PASSWORD_LEN];
    while (fgets(password, MAX_PASSWORD_LEN, file)) {
        // Remove the newline character from the password
        password[strcspn(password, "\n")] = 0;

        // Check if the password is correct
        if (check_zip_password(zip_file, password)) {
            print_key_found(password);
            fclose(file);
            return 1; // Found the correct password
        }
    }

    print_key_not_found();
    fclose(file);
    return 0; // Password not found
}

// Function to print the key found message
void print_key_found(const char *password) {
    printf("KEY FOUND: [ %s ]\n", password);
}

// Function to print the key not found message
void print_key_not_found() {
    printf("KEY NOT FOUND\n");
}
