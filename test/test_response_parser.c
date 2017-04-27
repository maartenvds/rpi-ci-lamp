#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "response_parser.h"

#define REGEX_MATCH_NOTHING "$a"
#define BIG_RESPONSE_STRING \
    "{\"branch\":{\"id\":214119164,\"repository_id\":119756,\"commit_id\":61565287,\"number\":\"4880\",\"config\"" \
    ":{\"language\":\"ruby\",\"sudo\":false,\"rvm\":\"2.3.3\",\"cache\":\"bundler\",\"addons\":{\"postgresql\"" \
    ":9.3},\"services\":[\"redis\"],\"before_script\":[\"RAILS_ENV=test bundle exec rake db:create--trace\"],\"" \
    ".result\":\"configured\",\"global_env\":[\"RUBY_GC_MALLOC_LIMIT=90000000\",\"RUBY_GC_HEAP_FREE_SLOTS=200000\"]," \
    "\"group\":\"stable\",\"dist\":\"precise\"},\"%s\":\"%s\",\"started_at\":\"2017-03-23T05:00:07Z\",\"" \
    "finished_at\":\"2017-03-23T05:04:30Z\",\"duration\":263,\"job_ids\":[214119165],\"pull_request\":false},\"" \
    "commit\":{\"id\":61565287,\"sha\":\"925ffdeea1a1013ec02b29c1b22ebbea6a851556\",\"branch\":\"master\",\"message\"" \
    ":\"Merge pull request #453 from travis-ci/meat-job-log-id\n\nRemoteLog serializationfixes\",\"committed_at\":\"" \
    "2017-03-22T23:00:34Z\",\"author_name\":\"Dan Buch\",\"author_email\":\"dan@meatballhat.com\",\"committer_name\"" \
    ":\"GitHub\",\"committer_email\":\"noreply@github.com\",\"compare_url\":\"https://github.com/travis-ci/travis-api" \
    "/compare/781f29346984d63f8e9e6b65e908e42d0c3a0fdb...925ffdeea1a1013ec02b29c1b22ebbea6a851556\"}}"

static void test_response_parser_build_passed(void **state)
{
    (void)state;
    int res;
    enum BuildState build_state;
    char *in;

    /* setup */
    res = asprintf(&in, BIG_RESPONSE_STRING, "state", "passed");
    assert_true(res > 0);

    /* act */
    res = response_parser_get_result(in, "\"state\":\"passed\"",
            REGEX_MATCH_NOTHING, REGEX_MATCH_NOTHING, &build_state);
    free(in);

    /* assert */
    assert_int_equal(res, 0);
    assert_int_equal(build_state, BUILD_STATE_PASSED);
}

static void test_response_parser_build_failed(void **state)
{
    (void)state;
    int res;
    enum BuildState build_state;
    char *in;

    /* setup */
    res = asprintf(&in, BIG_RESPONSE_STRING, "state", "failed");
    assert_true(res > 0);

    /* act */
    res = response_parser_get_result(in, REGEX_MATCH_NOTHING,
            REGEX_MATCH_NOTHING, "\"state\":\"failed\"", &build_state);
    free(in);

    /* assert */
    assert_int_equal(res, 0);
    assert_int_equal(build_state, BUILD_STATE_FAILED);
}

static void test_response_parser_build_is_running(void **state)
{
    (void)state;
    int res;
    enum BuildState build_state;
    char *in;

    /* setup */
    res = asprintf(&in, BIG_RESPONSE_STRING, "state", "created");
    assert_true(res > 0);

    /* act */
    res = response_parser_get_result(in, REGEX_MATCH_NOTHING,
            "\"state\":\"(started|created)\"", REGEX_MATCH_NOTHING, &build_state);
    free(in);

    /* assert */
    assert_int_equal(res, 0);
    assert_int_equal(build_state, BUILD_STATE_RUNNING);
}

static void test_response_parser_empty_response(void **state)
{
    (void)state;
    int res;
    enum BuildState build_state;
    char *in = "";

    /* act */
    res = response_parser_get_result(in, "\"state\":\"passed\"",
            REGEX_MATCH_NOTHING, REGEX_MATCH_NOTHING, &build_state);

    /* assert */
    assert_int_equal(res, -1);
}

static void test_response_parser_no_match(void **state)
{
    (void)state;
    int res;
    enum BuildState build_state;
    char *in = "no match in here";

    /* act */
    res = response_parser_get_result(in, REGEX_MATCH_NOTHING,
            REGEX_MATCH_NOTHING, REGEX_MATCH_NOTHING, &build_state);

    /* assert */
    assert_int_equal(res, -1);
}

static void test_response_parser_syntax_error_in_regex_passed(void **state)
{
    (void)state;
    int res;
    enum BuildState build_state;
    char *in = "some text";

    /* act */
    res = response_parser_get_result(in, "there|is|an|error|in)(here",
            REGEX_MATCH_NOTHING, REGEX_MATCH_NOTHING, &build_state);

    /* assert */
    assert_int_equal(res, -1);
}

static void test_response_parser_syntax_error_in_regex_running(void **state)
{
    (void)state;
    int res;
    enum BuildState build_state;
    char *in = "some text";

    /* act */
    res = response_parser_get_result(in, REGEX_MATCH_NOTHING,
            "there|is|an|error|in)(here", REGEX_MATCH_NOTHING, &build_state);

    /* assert */
    assert_int_equal(res, -1);
}

static void test_response_parser_syntax_error_in_regex_failed(void **state)
{
    (void)state;
    int res;
    enum BuildState build_state;
    char *in = "some text";

    /* act */
    res = response_parser_get_result(in, REGEX_MATCH_NOTHING,
            REGEX_MATCH_NOTHING, "there|is|an|error|in)(here", &build_state);

    /* assert */
    assert_int_equal(res, -1);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_response_parser_build_passed),
        cmocka_unit_test(test_response_parser_build_failed),
        cmocka_unit_test(test_response_parser_build_is_running),
        cmocka_unit_test(test_response_parser_empty_response),
        cmocka_unit_test(test_response_parser_no_match),
        cmocka_unit_test(test_response_parser_syntax_error_in_regex_passed),
        cmocka_unit_test(test_response_parser_syntax_error_in_regex_running),
        cmocka_unit_test(test_response_parser_syntax_error_in_regex_failed),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
