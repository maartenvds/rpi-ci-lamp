#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "lamp_io_red_green.h"
#include "application.h"

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
    (void)lamp_state;
}

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
    res = application_init(app, "dummy_filename", "dummy_uri", 0);
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
        cmocka_unit_test_setup_teardown(test_dummy, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
