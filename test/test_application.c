#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "application.h"

/*
 *  Test cases
 */
static void test_dummy(void **state)
{
    (void)state;
}

/*
 * Test fixtures
 */

static int setup(void **state)
{
    int res;
    struct Application *app = malloc(sizeof(struct Application));
    assert_non_null(app);
    res = application_init(app, "dummy_filename", "dummy_uri");
    assert_int_equal(res, -1);
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
        cmocka_unit_test_setup_teardown(test_dummy, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
