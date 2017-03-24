#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "settings_parser.h"

#define ENTRY_TEMPLATE          "{\"name\":\"%s\", \"branch\":\"%s\"}"
#define JSON_TEMPLATE           "{\"interval\":%s,\"repos\":[" ENTRY_TEMPLATE "]}"
#define JSON_MULTI_TEMPLATE     "{\"interval\":10,\"repos\":[" ENTRY_TEMPLATE ENTRY_TEMPLATE "]}"

static void test_settings_parser_one_build(void **state)
{
    (void) state;
    int res;
    char *in;
    struct Settings set;

    /* setup */
    res = asprintf(&in, JSON_TEMPLATE, "10", "owner/my_repo", "master");
    assert_true(res > 0);

    /* act */
    res = settings_parser_get_settings(in, &set);
    free(in);

    /* assert */
    assert_int_equal(res, 0);
    assert_int_equal(set.interval, 10);
    assert_int_equal(set.repo_count, 1);
    assert_string_equal(set.repos[0].name, "owner/my_repo");
    assert_string_equal(set.repos[0].branch, "master");
}

static void test_settings_parser_multi_build(void **state)
{
    (void) state;
    int res;
    char *in;
    struct Settings set;

    /* setup */
    res = asprintf(&in, JSON_MULTI_TEMPLATE, "owner/my_repo", "master", "owner/other_repo", "dev");
    assert_true(res > 0);

    /* act */
    res = settings_parser_get_settings(in, &set);
    free(in);

    /* assert */
    assert_int_equal(res, 0);
    assert_int_equal(set.interval, 10);
    assert_int_equal(set.repo_count, 2);
    assert_string_equal(set.repos[0].name, "owner/my_repo");
    assert_string_equal(set.repos[0].branch, "master");
    assert_string_equal(set.repos[1].name, "owner/other_repo");
    assert_string_equal(set.repos[1].branch, "dev");
}

static void test_settings_negative_interval(void **state)
{
    (void) state;
    int res;
    char *in;
    struct Settings set;

    /* setup */
    res = asprintf(&in, JSON_TEMPLATE, "-1", "owner/my_repo", "master");
    assert_true(res > 0);

    /* act */
    res = settings_parser_get_settings(in, &set);
    free(in);

    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_zero_interval(void **state)
{
    (void) state;
    int res;
    char *in;
    struct Settings set;

    /* setup */
    res = asprintf(&in, JSON_TEMPLATE, "0", "owner/my_repo", "master");
    assert_true(res > 0);

    /* act */
    res = settings_parser_get_settings(in, &set);
    free(in);

    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_non_numeric_interval(void **state)
{
    (void) state;
    int res;
    char *in;
    struct Settings set;

    /* setup */
    res = asprintf(&in, JSON_TEMPLATE, "one", "owner13/my_repo", "master");
    assert_true(res > 0);

    /* act */
    res = settings_parser_get_settings(in, &set);
    free(in);

    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_parser_order_indenendence(void **state)
{
    (void) state;
    int res;
    const char *in = "{\"repos\":[{\"branch\":\"master\",\"name\":\"owner/my_repo\"}], \"interval\":10}";
    struct Settings set;

    /* act */
    res = settings_parser_get_settings(in, &set);

    /* assert */
    assert_int_equal(res, 0);
    assert_int_equal(set.interval, 10);
    assert_int_equal(set.repo_count, 1);
    assert_string_equal(set.repos[0].name, "owner/my_repo");
    assert_string_equal(set.repos[0].branch, "master");
}

static void test_settings_syntax_error(void **state)
{
    (void) state;
    int res;
    const char *in = "{\"interval\"10,";
    struct Settings set;

    /* act */
    res = settings_parser_get_settings(in, &set);

    /* assert */
    assert_int_equal(res, -1);
}

static void test_settings_too_long_name(void **state)
{
    (void) state;
    int res;
    char *in;
    char too_long_name[SETTINGS_PARSER_STRING_SIZE + 10];
    struct Settings set;

    /* setup */
    memset(too_long_name, 'x', SETTINGS_PARSER_STRING_SIZE + 10);
    too_long_name[SETTINGS_PARSER_STRING_SIZE + 9] = '\0';
    res = asprintf(&in, JSON_TEMPLATE, "1", too_long_name, "master");
    assert_true(res > 0);

    /* act */
    res = settings_parser_get_settings(in, &set);
    free(in);

    /* assert */
    assert_int_equal(res, -1);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_settings_parser_one_build),
        cmocka_unit_test(test_settings_parser_multi_build),
        cmocka_unit_test(test_settings_negative_interval),
        cmocka_unit_test(test_settings_zero_interval),
        cmocka_unit_test(test_settings_non_numeric_interval),
        cmocka_unit_test(test_settings_parser_order_indenendence),
        cmocka_unit_test(test_settings_syntax_error),
        cmocka_unit_test(test_settings_too_long_name)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
