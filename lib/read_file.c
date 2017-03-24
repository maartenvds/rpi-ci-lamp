/*
 *  Read text file into buffer
 *  Author: Maarten Vandersteege
 */
#include "read_file.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *name)
{
    long int fsize;
    char *string;
    FILE *f = fopen(name, "rb");
    if (!f)
        return NULL;

    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    if (fsize < 0) {
        fclose(f);
        return NULL;
    }

    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    string = malloc((size_t)fsize + 1);
    if (!string)
        return NULL;

    fread(string, (size_t)fsize, 1, f);
    fclose(f);

    string[fsize] = 0;

    return string;
}
