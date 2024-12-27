#include "mephisto.h"

// Function to attempt extracting the ZIP file with a specific password
int try_password(const char *zip_file, const char *password) {
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));

    if (!mz_zip_reader_init_file(&zip, zip_file, 0)) {
        return 0;  // Failed to initialize ZIP reader
    }

    // Check for the first file in the archive
    int num_files = mz_zip_reader_get_num_files(&zip);
    if (num_files < 1) {
        mz_zip_reader_end(&zip);
        return 0;
    }

    // Try extracting with the given password
    for (int i = 0; i < num_files; ++i) {
        mz_zip_archive_file_stat file_stat;
        if (mz_zip_reader_file_stat(&zip, i, &file_stat)) {
            if (mz_zip_reader_is_file_encrypted(&zip, i)) {
                // Decrypt file with password
                if (mz_zip_reader_extract_to_callback(&zip, i, NULL, 0, NULL, NULL, password)) {
                    mz_zip_reader_end(&zip);
                    return 1;  // Password worked
                }
            }
        }
    }

    mz_zip_reader_end(&zip);
    return 0;  // Password didn't work
}

// Function to read password list and try each password
int crack_zip(const char *zip_file, const char *password_list) {
    FILE *file = fopen(password_list, "r");
    if (file == NULL) {
        return -1;  // Failed to open password list file
    }

    char password[MAX_PASSWORD_LEN];
    while (fgets(password, sizeof(password), file)) {
        // Remove newline character from password
        password[strcspn(password, "\n")] = 0;

        // Try the current password
        if (try_password(zip_file, password)) {
            printf("KEY FOUND: [%s]\n", password);
            fclose(file);
            return 1;  // Found the password
        }
    }

    fclose(file);
    printf("KEY NOT FOUND\n");
    return 0;  // No password found
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <zip_file> <password_list>\n", argv[0]);
        return 1;
    }

    const char *zip_file = argv[1];
    const char *password_list = argv[2];

    if (crack_zip(zip_file, password_list) == -1) {
        printf("Error opening password list\n");
        return 1;
    }

    return 0;
}
