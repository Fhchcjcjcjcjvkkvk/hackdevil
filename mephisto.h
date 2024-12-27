// mephisto.h

#ifndef MEPHISTO_H
#define MEPHISTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miniz.h"

// Function prototypes
int check_zip_password(const char *zip_filename, const char *password);
void crack_zip(const char *zip_filename, const char *password_list_file);

#endif // MEPHISTO_H
