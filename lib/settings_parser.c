/*
 *  Settings parser
 *  Author: Maarten Vandersteege
 */

#include "debug.h"
#include "settings_parser.h"
#include "jsmn.h"
#include <string.h>
#include <stdlib.h>

#define MAX_TOKENS  256
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

/*
 *  len = number of chars you want to copy
 *  size = size of destination buffer
 */
static void strlncpy(char *dest, const char *src, size_t len, size_t size)
{
    strncpy(dest, src, MIN(len, size));
    dest[size-1] = '\0';    /* guarantee null terminated string */
};

int settings_parser_get_settings(const char *in, struct Settings *out)
{
    int token_count, i;
    int got_name = 0;
    int got_branch = 0;
    jsmntok_t tokens[MAX_TOKENS];
    jsmn_parser parser;

    jsmn_init(&parser);
    token_count = jsmn_parse(&parser, in, strlen(in), tokens, MAX_TOKENS);

    if (token_count < 0) {
        debug("Failed to parse settings: %d\n", token_count);
        return -1;
    }

    out->repo_count = 0;

    for (i=0; i<token_count; i++) {

        if (JSMN_STRING != tokens[i].type)
            continue;

        if (strncmp(&in[tokens[i].start], "interval", tokens[i].end - tokens[i].start) == 0) {
            i++;
            out->interval = atoi(&in[tokens[i].start]);

        } else if (strncmp(&in[tokens[i].start], "name", tokens[i].end - tokens[i].start) == 0) {
            i++;
            strlncpy(out->repos[out->repo_count].name, &in[tokens[i].start],
                                    tokens[i].end - tokens[i].start, SETTINGS_PARSER_STRING_SIZE);
            got_name = 1;

        } else if (strncmp(&in[tokens[i].start], "branch", tokens[i].end - tokens[i].start) == 0) {
            i++;
            strlncpy(out->repos[out->repo_count].branch, &in[tokens[i].start],
                                    tokens[i].end - tokens[i].start, SETTINGS_PARSER_STRING_SIZE);
            got_branch = 1;
        }

        if (got_name && got_branch) {
            out->repo_count++;
            got_name = got_branch = 0;
        }
    }

    return 0;
}
