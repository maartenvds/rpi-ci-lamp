#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include "settings_parser.h"

static void test_settings_parser_one_build(void **state)
{
    (void) state;
    int res;
    const char *in = "{\"interval\":10,\"repos\":[{\"name\":\"owner/my_repo\", \"branch\":\"master\"}]}";
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
    int res, i;
    char in[SETTINGS_PARSER_STRING_SIZE+100] = "{\"interval\":10,\"repos\":[{\"name\":\"";
    const char *in_end = "\", \"branch\":\"master\"}]}";
    struct Settings set;

    /* setup */
    /* generate a name that is 10 characters too long */
    for (i=0; i<SETTINGS_PARSER_STRING_SIZE+10; i++)
        strcat(in, "x");
    strcat(in, in_end); /* finish the string */

    /* act */
    res = settings_parser_get_settings(in, &set);

    /* assert */
    assert_int_equal(res, 0);
    assert_int_equal(set.repo_count, 1);
    assert_int_equal(strlen(set.repos[0].name), SETTINGS_PARSER_STRING_SIZE-1);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_settings_parser_one_build),
        cmocka_unit_test(test_settings_parser_order_indenendence),
        cmocka_unit_test(test_settings_syntax_error),
        cmocka_unit_test(test_settings_too_long_name)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
