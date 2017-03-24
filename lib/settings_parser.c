/*
 *  Settings parser
 *  Author: Maarten Vandersteege
 */

#include "logging.h"
#include "settings_parser.h"
#include "jsmn.h"
#include <string.h>
#include <stdlib.h>

#define MAX_TOKENS  256

/*
 *  assumes size of dest = SETTINGS_PARSER_STRING_SIZE
 *  returns -1 when token string is truncated in dest because it is longer
 */
static int copy_token(char *dest, const char *json, jsmntok_t token)
{
    size_t len = token.end - token.start;
    const char *in = &json[token.start];

    if (len >= SETTINGS_PARSER_STRING_SIZE) {
        strncpy(dest, in, SETTINGS_PARSER_STRING_SIZE);
        dest[SETTINGS_PARSER_STRING_SIZE-1] = '\0';
        return -1;
    }

    strncpy(dest, in, len);
    dest[len] = '\0';

    return 0;
};

int settings_parser_get_settings(const char *in, struct Settings *out)
{
    int token_count, i;
    int got_name = 0;
    int got_branch = 0;
    jsmntok_t tokens[MAX_TOKENS];
    char token_str[SETTINGS_PARSER_STRING_SIZE];
    jsmn_parser parser;

    jsmn_init(&parser);
    token_count = jsmn_parse(&parser, in, strlen(in), tokens, MAX_TOKENS);

    if (token_count < 0) {
        error("Failed to parse settings: %d\n", token_count);
        return -1;
    }

    out->repo_count = 0;

    for (i=0; i<token_count; i++) {

        if (JSMN_STRING != tokens[i].type)
            continue;

        (void)copy_token(token_str, in, tokens[i]);

        if (strcmp(token_str, "interval") == 0) {
            i++;
            (void)copy_token(token_str, in, tokens[i]);
            out->interval = atoi(token_str);

            if (out->interval <= 0) {
                error("Settings interval cannot be <= zero\n");
                return -1;
            }

        } else if (out->repo_count == SETTINGS_PARSER_REPO_SIZE) {
            continue;

        } else if (strcmp(token_str, "name") == 0) {
            i++;
            if (copy_token(out->repos[out->repo_count].name, in, tokens[i]) == -1) {
                error("Max string length exceeded, truncated 'name' %s\n", token_str);
                return -1;
            }
            got_name = 1;

        } else if (strcmp(token_str, "branch") == 0) {
            i++;
            if (copy_token(out->repos[out->repo_count].branch, in, tokens[i]) == -1) {
                error("Max string length exceeded, truncated 'branch' %s\n", token_str);
                return -1;
            }
            got_branch = 1;
        }

        if (got_name && got_branch) {
            got_name = got_branch = 0;
            out->repo_count++;
        }
    }

    return 0;
}
