#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "lamp_io_red_green.h"
#include "application.h"
#include <unistd.h>

#define LOCALHOST_SERVER_URI    "https://localhost:4443"
#define FILE_PREFIX             "test/data/"

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
 *  Test cases
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

/*
 * Test fixtures
 */

static int setup_state(void **state)
{
    int res;
    struct Application *app = malloc(sizeof(struct Application));
    assert_non_null(app);

    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_OFF);
    res = application_init(app, FILE_PREFIX "settings.json", LOCALHOST_SERVER_URI, 1);
    assert_int_equal(res, 0);

    *state = app;
    return 0;
}

static int teardown_state(void **state)
{
    application_deinit(*state);
    free(*state);
    return 0;
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_build_passed, setup_state, teardown_state)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
