#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cracker.h"

#define MAX_PASSWORD_LENGTH 256

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <zip_file> -P <password_list>\n", argv[0]);
        return 1;
    }

    const char *zip_filename = argv[1];
    const char *password_list_filename = argv[3];

    FILE *password_file = fopen(password_list_filename, "r");
    if (!password_file) {
        perror("Error opening password list");
        return 1;
    }

    char password[MAX_PASSWORD_LENGTH];

    while (fgets(password, MAX_PASSWORD_LENGTH, password_file)) {
        // Remove trailing newline character
        password[strcspn(password, "\n")] = 0;

        if (try_password(zip_filename, password)) {
            print_key_found(password);
            fclose(password_file);
            return 0;
        }
    }

    print_key_not_found();
    fclose(password_file);
    return 0;
}
