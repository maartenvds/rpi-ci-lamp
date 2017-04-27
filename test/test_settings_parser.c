#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "settings_parser.h"

#define SETTINGS_FILE               "test_settings.conf"

/*
 *  MACROS to contruct settings file content
 */

#define URL                         "https://localhost:4443/repos/maartenvds/rpi-ci-lamp/branches/master"
#define REGEX_PASSED                "state:passed"
#define REGEX_RUNNING               "state:(created|started)"
#define REGEX_FAILED                "state:failed"

#define SETTING_INTERVAL            \
    "interval = 5;\n"

#define SETTING_SERVER_START        \
    "travis-org = {\n"

#define SETTING_SERVER_HEADERS      \
    "   headers = (\"User-Agent: MyClient/1.0.0\",\n" \
    "               \"Accept: application/vnd.travis-ci.2+json\",\n" \
    "               \"Host: api.travis-ci.org\");\n"

#define SETTING_SERVER_EMPTY_HEADERS    \
    "   headers = ();"

#define SETTING_SERVER_HEADERS_WRONG_TYPE   \
    "   headers = (\"a string\", 3, true);"

#define SETTING_SERVER_REGEX_PASSED \
    "   regex_passed = \"" REGEX_PASSED "\";\n"

#define SETTING_SERVER_REGEX_RUNNING \
    "   regex_running = \"" REGEX_RUNNING "\";\n"

#define SETTING_SERVER_REGEX_FAILED \
    "   regex_failed = \"" REGEX_FAILED "\";\n"

#define SETTING_SERVER_END          \
    "};\n"

#define SETTING_BUILD_INFO_START    \
    "build-info = (\n"

#define SETTING_BUILD_INFO_ENTRY    \
    "   {\n" \
    "       server = \"travis-org\";\n" \
    "       url = \"" URL "\";\n" \
    "   }"

#define SETTING_BUILD_INFO_ENTRY_MISSING_URL \
    "   {\n" \
    "       server = \"travis-org\";\n" \
    "   }\n"

#define SETTING_BUILD_INFO_ENTRY_MISSING_SERVER \
    "   {\n" \
    "       url = \"" URL "\";\n" \
    "   }\n"

#define SETTING_BUILD_INFO_ENTRY_INTERMEDIATE   \
    SETTING_BUILD_INFO_ENTRY ",\n"

#define SETTING_BUILD_INFO_ENTRY_LAST   \
    SETTING_BUILD_INFO_ENTRY "\n"

#define SETTING_BUILD_INFO_END      \
    ");\n"

#define SETTING_SERVER              \
    SETTING_SERVER_START \
    SETTING_SERVER_HEADERS \
    SETTING_SERVER_REGEX_PASSED \
    SETTING_SERVER_REGEX_RUNNING \
    SETTING_SERVER_REGEX_FAILED \
    SETTING_SERVER_END

#define SETTING_BUILD_INFO_SINGLE   \
    SETTING_BUILD_INFO_START \
    SETTING_BUILD_INFO_ENTRY_LAST \
    SETTING_BUILD_INFO_END

#define SETTING_BUILD_INFO_MULTI    \
    SETTING_BUILD_INFO_START \
    SETTING_BUILD_INFO_ENTRY_INTERMEDIATE \
    SETTING_BUILD_INFO_ENTRY_LAST \
    SETTING_BUILD_INFO_END \

//TODO: expand using macro magic
#define SETTING_BUILD_INFO_TOO_MANY_ENTRIES \
    SETTING_BUILD_INFO_START \
    SETTING_BUILD_INFO_ENTRY_INTERMEDIATE \
    SETTING_BUILD_INFO_ENTRY_INTERMEDIATE \
    SETTING_BUILD_INFO_ENTRY_INTERMEDIATE \
    SETTING_BUILD_INFO_ENTRY_INTERMEDIATE \
    SETTING_BUILD_INFO_ENTRY_INTERMEDIATE \
    SETTING_BUILD_INFO_ENTRY_INTERMEDIATE \
    SETTING_BUILD_INFO_ENTRY_INTERMEDIATE \
    SETTING_BUILD_INFO_ENTRY_LAST \
    SETTING_BUILD_INFO_END

#define SETTINGS_SYNTAX_ERROR       \
"}interval = 5;{\n"

#define SETTING_BUILD_INFO_MISSING_URL  \
    SETTING_BUILD_INFO_START \
    SETTING_BUILD_INFO_ENTRY_MISSING_URL \
    SETTING_BUILD_INFO_END

#define SETTING_BUILD_INFO_MISSING_SERVER  \
    SETTING_BUILD_INFO_START \
    SETTING_BUILD_INFO_ENTRY_MISSING_SERVER \
    SETTING_BUILD_INFO_END

/*
 *  Helper functions
 */
static void create_settings_file(const char *content)
{
    FILE *f;
    size_t res;

    f = fopen(SETTINGS_FILE, "w");
    res = fwrite(content, 1, strlen(content), f);
    assert_true(res > 0);
    fclose(f);
}

/*
 *  Bad weather test cases
 */
static void test_settings_non_existing_settings_file(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* act */
    res = settings_parser_parse_config("nonexistingfile", &settings);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_parser_syntax_error(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* setup */
    create_settings_file(SETTINGS_SYNTAX_ERROR);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, &settings);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_missing_interval_setting(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* setup */
    create_settings_file(SETTING_BUILD_INFO_SINGLE);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, &settings);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_missing_build_info_setting(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* setup */
    create_settings_file(SETTING_INTERVAL);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, &settings);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_too_many_build_info_settings(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* setup */
    create_settings_file(SETTING_INTERVAL SETTING_BUILD_INFO_TOO_MANY_ENTRIES SETTING_SERVER);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, &settings);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_build_info_missing_url(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* setup */
    create_settings_file(SETTING_INTERVAL SETTING_BUILD_INFO_MISSING_URL SETTING_SERVER);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, &settings);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_build_info_missing_server(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* setup */
    create_settings_file(SETTING_INTERVAL SETTING_BUILD_INFO_MISSING_SERVER SETTING_SERVER);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, &settings);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_server_settings_not_found(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* setup */
    create_settings_file(SETTING_INTERVAL SETTING_BUILD_INFO_SINGLE);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, &settings);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_server_settings_regex_passed_not_found(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* setup */
    create_settings_file(SETTING_INTERVAL SETTING_BUILD_INFO_SINGLE \
                            SETTING_SERVER_START \
                            SETTING_SERVER_HEADERS \
                            SETTING_SERVER_REGEX_RUNNING \
                            SETTING_SERVER_REGEX_FAILED \
                            SETTING_SERVER_END);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, &settings);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_server_settings_regex_running_not_found(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* setup */
    create_settings_file(SETTING_INTERVAL SETTING_BUILD_INFO_SINGLE \
                            SETTING_SERVER_START \
                            SETTING_SERVER_HEADERS \
                            SETTING_SERVER_REGEX_PASSED \
                            SETTING_SERVER_REGEX_FAILED \
                            SETTING_SERVER_END);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, &settings);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_server_settings_regex_failed_not_found(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* setup */
    create_settings_file(SETTING_INTERVAL SETTING_BUILD_INFO_SINGLE \
                            SETTING_SERVER_START \
                            SETTING_SERVER_HEADERS \
                            SETTING_SERVER_REGEX_PASSED \
                            SETTING_SERVER_REGEX_RUNNING \
                            SETTING_SERVER_END);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, &settings);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_server_settings_headers_not_found(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* setup */
    create_settings_file(SETTING_INTERVAL SETTING_BUILD_INFO_SINGLE \
                            SETTING_SERVER_START \
                            SETTING_SERVER_REGEX_PASSED \
                            SETTING_SERVER_REGEX_RUNNING \
                            SETTING_SERVER_REGEX_FAILED \
                            SETTING_SERVER_END);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, &settings);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_server_settings_headers_wrong_type(void **state)
{
    (void)state;
    int res;
    struct Settings settings;

    /* setup */
    create_settings_file(SETTING_INTERVAL SETTING_BUILD_INFO_SINGLE \
                            SETTING_SERVER_START \
                            SETTING_SERVER_HEADERS_WRONG_TYPE \
                            SETTING_SERVER_REGEX_PASSED \
                            SETTING_SERVER_REGEX_RUNNING \
                            SETTING_SERVER_REGEX_FAILED \
                            SETTING_SERVER_END);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, &settings);
    /* assert */
    assert_int_equal(res, -1);
}

/*
 *  Good weather test cases
 */

static void test_settings_server_settings_empty_header_list(void **state)
{
    int res;
    struct Settings *settings = *state;

    /* setup */
    create_settings_file(SETTING_INTERVAL SETTING_BUILD_INFO_SINGLE \
                            SETTING_SERVER_START \
                            SETTING_SERVER_EMPTY_HEADERS \
                            SETTING_SERVER_REGEX_PASSED \
                            SETTING_SERVER_REGEX_RUNNING \
                            SETTING_SERVER_REGEX_FAILED \
                            SETTING_SERVER_END);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, settings);
    /* assert */
    assert_int_equal(res, 0);
    assert_int_equal(settings->builds_count, 1);
    assert_null(settings->builds[0].headers);
}

static void test_settings_server_settings_single_build_info_entry(void **state)
{
    int res;
    struct Settings *settings = *state;

    /* setup */
    create_settings_file(SETTING_INTERVAL SETTING_BUILD_INFO_SINGLE SETTING_SERVER);
    /* act */
    res = settings_parser_parse_config(SETTINGS_FILE, settings);
    /* assert */
    assert_int_equal(res, 0);
    assert_int_equal(settings->interval, 5);
    assert_int_equal(settings->builds_count, 1);
    assert_non_null(settings->builds[0].headers);
    assert_string_equal(settings->builds[0].url, URL);
    assert_string_equal(settings->builds[0].regex_passed, REGEX_PASSED);
    assert_string_equal(settings->builds[0].regex_running, REGEX_RUNNING);
    assert_string_equal(settings->builds[0].regex_failed, REGEX_FAILED);
}

/*
 *  Test fixtures
 */
static int setup(void **state)
{
    struct Settings *settings = malloc(sizeof(struct Settings));
    *state = settings;
    return 0;
}

static int teardown(void **state)
{
    (void)remove(SETTINGS_FILE);
    settings_parser_destroy(*state);
    free(*state);
    return 0;
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_settings_non_existing_settings_file),
        cmocka_unit_test(test_settings_parser_syntax_error),
        cmocka_unit_test(test_settings_missing_interval_setting),
        cmocka_unit_test(test_settings_missing_build_info_setting),
        cmocka_unit_test(test_settings_too_many_build_info_settings),
        cmocka_unit_test(test_settings_build_info_missing_url),
        cmocka_unit_test(test_settings_build_info_missing_server),
        cmocka_unit_test(test_settings_server_settings_not_found),
        cmocka_unit_test(test_settings_server_settings_regex_passed_not_found),
        cmocka_unit_test(test_settings_server_settings_regex_running_not_found),
        cmocka_unit_test(test_settings_server_settings_regex_failed_not_found),
        cmocka_unit_test(test_settings_server_settings_headers_not_found),
        cmocka_unit_test(test_settings_server_settings_headers_wrong_type),
        cmocka_unit_test_setup_teardown(test_settings_server_settings_empty_header_list, setup, teardown),
        cmocka_unit_test_setup_teardown(test_settings_server_settings_single_build_info_entry, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
