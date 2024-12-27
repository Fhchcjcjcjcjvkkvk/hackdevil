#ifndef CRACKER_H
#define CRACKER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <miniz.h>

// Function prototypes
int try_password(const char *zip_filename, const char *password);
void print_key_found(const char *password);
void print_key_not_found();

#endif
