/*
 *  Read small text file into buffer
 *  Author: Maarten Vandersteege
 */
#include "read_file.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *name)
{
    long int fsize;
    char *string;
    FILE *f;

    f = fopen(name, "rb");
    if (!f)
        return NULL;

    /* calc file size */
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    if (fsize < 0) {
        fclose(f);
        return NULL;
    }

    string = malloc((size_t)fsize + 1);
    if (!string) {
        fclose(f);
        return NULL;
    }

    rewind(f);
    if (fread(string, (size_t)fsize, 1, f) != 1) {
        free(string);
        fclose(f);
        return NULL;
    }

    fclose(f);
    string[fsize] = 0;

    return string;
}
