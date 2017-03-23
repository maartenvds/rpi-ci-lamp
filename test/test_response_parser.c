#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "response_parser.h"

#define BIG_RESPONSE_STRING \
    "HTTP/1.1 %s OK\r\n" \
    "Connection: keep-alive\r\n" \
    "Server: nginx\r\n" \
    "Date: Thu, 23 Mar 2017 14:34:55 GMT\r\n" \
    "Content-Type: application/json;charset=utf-8\r\n" \
    "Content-Length: 1133\r\n" \
    "Access-Control-Allow-Origin: *\r\n" \
    "Access-Control-Allow-Credentials: true\r\n" \
    "Access-Control-Expose-Headers: Content-Type, Cache-Control, Expires, Etag, Last-Modified\r\n" \
    "Strict-Transport-Security: max-age=31536000\r\n" \
    "X-Endpoint: Travis::Api::App::Endpoint::Repos\r\n" \
    "X-Pattern: /:owner_name/:name/branches/*\r\n" \
    "X-Oauth-Scopes: public\r\n" \
    "X-Accepted-Oauth-Scopes: public\r\n" \
    "Vary: Accept,Accept-Encoding\r\n" \
    "Cache-Control: no-cache\r\n" \
    "Etag: \"35df894ec5c29956cff5a6b60b68b374\"\r\n" \
    "X-Content-Digest: f202e0ba7b10da961766be41a9c22d17500ae6d8\r\n" \
    "Age: 0\r\n" \
    "X-Rack-Cache: miss, store\r\n" \
    "Via: 1.1 vegur\r\n" \
    "%s" \
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

static void test_response_parser_passed(void **state)
{
    int res;
    int passed;
    char *in = *state;

    /* setup */
    sprintf(in, BIG_RESPONSE_STRING, "200", "\r\n", "state", "passed");

    /* act */
    res = response_parser_build_result(in, &passed);

    /* assert */
    assert_int_equal(res, 0);
    assert_true(passed);
}

static void test_response_parser_failed(void **state)
{
    int res;
    int passed;
    char *in = *state;

    /* setup */
    sprintf(in, BIG_RESPONSE_STRING, "200", "\r\n", "state", "failed");

    /* act */
    res = response_parser_build_result(in, &passed);

    /* assert */
    assert_int_equal(res, 0);
    assert_false(passed);
}

static void test_response_parser_bad_reponse_code(void **state)
{
    int res;
    int passed;
    char *in = *state;

    /* setup */
    sprintf(in, BIG_RESPONSE_STRING, "404", "\r\n", "state", "failed");

    /* act */
    res = response_parser_build_result(in, &passed);

    /* assert */
    assert_int_equal(res, -1);
}

static void test_response_parser_end_of_http_header_not_found(void **state)
{
    int res;
    int passed;
    char *in = *state;

    /* setup */
    sprintf(in, BIG_RESPONSE_STRING, "200", "", "state", "failed");

    /* act */
    res = response_parser_build_result(in, &passed);

    /* assert */
    assert_int_equal(res, -1);
}

static void test_response_parser_state_token_not_found(void **state)
{
    int res;
    int passed;
    char *in = *state;

    /* setup */
    sprintf(in, BIG_RESPONSE_STRING, "200", "\r\n", "unknown", "failed");

    /* act */
    res = response_parser_build_result(in, &passed);

    /* assert */
    assert_int_equal(res, -1);
}

static int setup(void **state)
{
    char *str = malloc(strlen(BIG_RESPONSE_STRING) + 100);
    assert_non_null(str);
    *state = str;
    return 0;
}

static int teardown(void **state)
{
    free(*state);
    return 0;
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_response_parser_passed, setup, teardown),
        cmocka_unit_test_setup_teardown(test_response_parser_failed, setup, teardown),
        cmocka_unit_test_setup_teardown(test_response_parser_bad_reponse_code, setup, teardown),
        cmocka_unit_test_setup_teardown(test_response_parser_end_of_http_header_not_found, setup, teardown),
        cmocka_unit_test_setup_teardown(test_response_parser_state_token_not_found, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
