#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "lamp_io_red_green.h"
#include "application.h"
#include <unistd.h>

#define LOCALHOST_TEST_SERVER_URI   "https://localhost:4443"
#define FILE_PREFIX                 "test/data/"
#define SETTINGS_FILE               FILE_PREFIX "settings.json"

/*
 *  Mocks
 */

int lamp_io_red_green_init(struct LampIoRedGreen *self)
{
    (void)self;
    return 0;
}

void lamp_io_red_green_deinit(struct LampIoRedGreen *self)
{
    (void)self;
}

void lamp_io_red_green_set_state(struct LampIoRedGreen *self, enum LampStateRedGreen lamp_state)
{
    (void)self;
    check_expected(lamp_state);
}

/*
 *  Helper functions
 *  converts 'target' to an absolute path
 *  if symlinkfile already exists, it is first removed and re-created
 */
static void set_symlink(const char *target, const char *symlinkfile)
{
    int res;
    char cwd[1024];
    char *abs_target;

    assert_true(getcwd(cwd, sizeof(cwd)) != NULL);

    res = asprintf(&abs_target, "%s/%s", cwd, target);
    assert_true(res > 0);

    res = symlink(abs_target, symlinkfile);

    if (res != 0) {
        res = remove(symlinkfile);
        assert_int_equal(res, 0);
        res = symlink(abs_target, symlinkfile);
        assert_int_equal(res, 0);
    }

    free(abs_target);
}

/*
 *  Good weather test cases
 */

static void test_build_passed(void **state)
{
    struct Application *app = *state;

    /* setup */
    set_symlink(FILE_PREFIX "content_passed.txt", FILE_PREFIX "master");
    set_symlink(FILE_PREFIX "single_repo_settings.json", FILE_PREFIX "settings.json");
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_GREEN);

    /* act */
    (void) application_run(app);
}

static void test_build_running(void **state)
{
    struct Application *app = *state;

    /* setup */
    set_symlink(FILE_PREFIX "content_running.txt", FILE_PREFIX "master");
    set_symlink(FILE_PREFIX "single_repo_settings.json", FILE_PREFIX "settings.json");
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_BLINK_GREEN);

    /* act */
    (void) application_run(app);
}

static void test_build_failed(void **state)
{
    struct Application *app = *state;

    /* setup */
    set_symlink(FILE_PREFIX "content_failed.txt", FILE_PREFIX "master");
    set_symlink(FILE_PREFIX "single_repo_settings.json", FILE_PREFIX "settings.json");
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_RED);

    /* act */
    (void) application_run(app);
}

static void test_build_passed_multi(void **state)
{
    struct Application *app = *state;

    /* setup */
    set_symlink(FILE_PREFIX "content_passed.txt", FILE_PREFIX "master");
    set_symlink(FILE_PREFIX "content_passed.txt", FILE_PREFIX "dev");
    set_symlink(FILE_PREFIX "multi_repo_settings.json", FILE_PREFIX "settings.json");
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_GREEN);

    /* act */
    (void) application_run(app);
}

static void test_build_running_multi(void **state)
{
    struct Application *app = *state;

    /* setup */
    set_symlink(FILE_PREFIX "content_failed.txt", FILE_PREFIX "master");
    set_symlink(FILE_PREFIX "content_running.txt", FILE_PREFIX "dev");
    set_symlink(FILE_PREFIX "multi_repo_settings.json", FILE_PREFIX "settings.json");
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_BLINK_GREEN);

    /* act */
    (void) application_run(app);
}

static void test_build_failed_multi(void **state)
{
    struct Application *app = *state;

    /* setup */
    set_symlink(FILE_PREFIX "content_passed.txt", FILE_PREFIX "master");
    set_symlink(FILE_PREFIX "content_failed.txt", FILE_PREFIX "dev");
    set_symlink(FILE_PREFIX "multi_repo_settings.json", FILE_PREFIX "settings.json");
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_RED);

    /* act */
    (void) application_run(app);
}

/*
 *  Bad weather
 */

static void test_settings_file_not_found(void **state)
{
    int res;
    struct Application *app = *state;

    /* setup */
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_OFF);
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_ERROR);

    /* act */
    res = application_init(app, "nonexistingfile", LOCALHOST_TEST_SERVER_URI, 1);
    assert_int_equal(res, 0);
    (void) application_run(app);
}

static void test_settings_file_syntax_error(void **state)
{
    int res;
    struct Application *app = *state;

    /* setup */
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_OFF);
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_ERROR);

    /* act */
    res = application_init(app, FILE_PREFIX "syntax_error_settings.txt", LOCALHOST_TEST_SERVER_URI, 1);
    assert_int_equal(res, 0);
    (void) application_run(app);
}

static void test_server_uri_non_existing(void **state)
{
    int res;
    struct Application *app = *state;

    /* setup */
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_OFF);
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_ERROR);

    /* act */
    res = application_init(app, SETTINGS_FILE, "non.existing.server.uri", 1);
    assert_int_equal(res, 0);
    (void) application_run(app);
}

static void test_server_certificate_check_failed(void **state)
{
    int res;
    struct Application *app = *state;

    /* setup */
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_OFF);
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_ERROR);

    /* act */
    /* test server is self signed so certificate check will fail when enabled */
    res = application_init(app, SETTINGS_FILE, LOCALHOST_TEST_SERVER_URI, 0);
    assert_int_equal(res, 0);
    (void) application_run(app);
}

static void test_unexisting_branch_name_in_settings_file(void **state)
{
    int res;
    struct Application *app = *state;

    /* setup */
    set_symlink(FILE_PREFIX "content_error.txt", FILE_PREFIX "master");
    set_symlink(FILE_PREFIX "single_repo_settings.json", FILE_PREFIX "settings.json");
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_OFF);
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_ERROR);

    /* act */
    res = application_init(app, SETTINGS_FILE, LOCALHOST_TEST_SERVER_URI, 1);
    assert_int_equal(res, 0);
    (void) application_run(app);
}

/*
 * Test fixtures
 */

static int setup_bw(void **state)
{
    struct Application *app = malloc(sizeof(struct Application));
    assert_non_null(app);
    *state = app;
    return 0;
}

static int setup_gw(void **state)
{
    int res;
    struct Application *app = malloc(sizeof(struct Application));
    assert_non_null(app);

    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_OFF);
    res = application_init(app, SETTINGS_FILE, LOCALHOST_TEST_SERVER_URI, 1);
    assert_int_equal(res, 0);

    *state = app;
    return 0;
}

static int teardown(void **state)
{
    application_deinit(*state);
    free(*state);
    return 0;
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_build_passed, setup_gw, teardown),
        cmocka_unit_test_setup_teardown(test_build_running, setup_gw, teardown),
        cmocka_unit_test_setup_teardown(test_build_failed, setup_gw, teardown),
        cmocka_unit_test_setup_teardown(test_build_passed_multi, setup_gw, teardown),
        cmocka_unit_test_setup_teardown(test_build_running_multi, setup_gw, teardown),
        cmocka_unit_test_setup_teardown(test_build_failed_multi, setup_gw, teardown),
        cmocka_unit_test_setup_teardown(test_settings_file_not_found, setup_bw, teardown),
        cmocka_unit_test_setup_teardown(test_server_uri_non_existing, setup_bw, teardown),
        cmocka_unit_test_setup_teardown(test_server_certificate_check_failed, setup_bw, teardown),
        cmocka_unit_test_setup_teardown(test_unexisting_branch_name_in_settings_file, setup_bw, teardown),
        cmocka_unit_test_setup_teardown(test_settings_file_syntax_error, setup_bw, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
