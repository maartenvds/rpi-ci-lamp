#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *name)
{
    long fsize;
    char *string;
    FILE *f = fopen(name, "rb");
    if (!f)
        return NULL;

    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    string = malloc(fsize + 1);
    if (!string)
        return NULL;

    fread(string, fsize, 1, f);
    fclose(f);

    string[fsize] = 0;

    return string;
}
