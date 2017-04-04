#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "lamp_control.h"
#include "lamp_io_red_green.h"

/*
 * Mocks
 */
void lamp_io_init(void)
{
}

void lamp_io_set_state(int lamp_state)
{
    check_expected(lamp_state);
}

/*
 * Tests
 *
 * Transition table of expected transitions:
 *
 *  rows =          current lamp state
 *  cols =          given build state
 *  cell values =   new lamp state
 *
 *                  failed  running         passed
 *
 *  green           red     green blink     green
 *  red             red     red blink       green
 *  green blink     red     green blink     green
 *  red blink       red     red blink       green
 *  off             red     green blink     green
 *  error           red     red blink       green
 *
 */
static void test_set_state_build_failed(void **state)
{
    int *lamp_state = *state;

    /* setup */
    expect_value(lamp_io_set_state, lamp_state, LAMP_STATE_RED);
    /* act */
    lamp_control_set_state(lamp_state, BUILD_STATE_FAILED);
    /* assert */
    assert_int_equal(*lamp_state, LAMP_STATE_RED);
}

static void test_set_state_build_passed(void **state)
{
    int *lamp_state = *state;

    /* setup */
    expect_value(lamp_io_set_state, lamp_state, LAMP_STATE_GREEN);
    /* act */
    lamp_control_set_state(lamp_state, BUILD_STATE_PASSED);
    /* assert */
    assert_int_equal(*lamp_state, LAMP_STATE_GREEN);
}

static void test_set_state_build_running_after_failed(void **state)
{
    int *lamp_state = *state;

    /* setup */
    *lamp_state = LAMP_STATE_RED;
    expect_value(lamp_io_set_state, lamp_state, LAMP_STATE_BLINK_RED);
    /* act */
    lamp_control_set_state(lamp_state, BUILD_STATE_RUNNING);
    /* assert */
    assert_int_equal(*lamp_state, LAMP_STATE_BLINK_RED);
}

static void test_set_state_build_running_after_passed(void **state)
{
    int *lamp_state = *state;

    /* setup */
    *lamp_state = LAMP_STATE_GREEN;
    expect_value(lamp_io_set_state, lamp_state, LAMP_STATE_BLINK_GREEN);
    /* act */
    lamp_control_set_state(lamp_state, BUILD_STATE_RUNNING);
    /* assert */
    assert_int_equal(*lamp_state, LAMP_STATE_BLINK_GREEN);
}

static void test_set_state_build_running_after_off(void **state)
{
    int *lamp_state = *state;

    /* setup */
    expect_value(lamp_io_set_state, lamp_state, LAMP_STATE_BLINK_GREEN);
    /* act */
    lamp_control_set_state(lamp_state, BUILD_STATE_RUNNING);
    /* assert */
    assert_int_equal(*lamp_state, LAMP_STATE_BLINK_GREEN);
}

static void test_set_state_build_running_after_error(void **state)
{
    int *lamp_state = *state;

    /* setup */
    *lamp_state = LAMP_STATE_ERROR;
    expect_value(lamp_io_set_state, lamp_state, LAMP_STATE_BLINK_RED);
    /* act */
    lamp_control_set_state(lamp_state, BUILD_STATE_RUNNING);
    /* assert */
    assert_int_equal(*lamp_state, LAMP_STATE_BLINK_RED);
}

static void test_set_state_build_running_after_blink_green(void **state)
{
    int *lamp_state = *state;

    /* setup */
    *lamp_state = LAMP_STATE_BLINK_GREEN;
    expect_value(lamp_io_set_state, lamp_state, LAMP_STATE_BLINK_GREEN);
    /* act */
    lamp_control_set_state(lamp_state, BUILD_STATE_RUNNING);
    /* assert */
    assert_int_equal(*lamp_state, LAMP_STATE_BLINK_GREEN);
}

static void test_set_state_build_running_after_blink_red(void **state)
{
    int *lamp_state = *state;

    /* setup */
    *lamp_state = LAMP_STATE_BLINK_RED;
    expect_value(lamp_io_set_state, lamp_state, LAMP_STATE_BLINK_RED);
    /* act */
    lamp_control_set_state(lamp_state, BUILD_STATE_RUNNING);
    /* assert */
    assert_int_equal(*lamp_state, LAMP_STATE_BLINK_RED);
}

static void test_signal_error(void **state)
{
    int *lamp_state = *state;

    /* setup */
    expect_value(lamp_io_set_state, lamp_state, LAMP_STATE_ERROR);
    /* act */
    lamp_control_signal_error(lamp_state);
    /* assert */
    assert_int_equal(*lamp_state, LAMP_STATE_ERROR);
}

static void test_lamp_off(void **state)
{
    int *lamp_state = *state;

    /* setup */
    *lamp_state = LAMP_STATE_RED;
    expect_value(lamp_io_set_state, lamp_state, LAMP_STATE_OFF);
    /* act */
    lamp_control_off(lamp_state);
    /* assert */
    assert_int_equal(*lamp_state, LAMP_STATE_OFF);
}

/*
 * Test fixtures
 */

static int setup(void **state)
{
    int *lamp_state = malloc(sizeof(int));
    assert_non_null(lamp_state);
    *state = lamp_state;

    expect_value(lamp_io_set_state, lamp_state, LAMP_STATE_OFF);
    lamp_control_init(lamp_state);
    assert_int_equal(*lamp_state, LAMP_STATE_OFF);

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
        cmocka_unit_test_setup_teardown(test_set_state_build_failed, setup, teardown),
        cmocka_unit_test_setup_teardown(test_set_state_build_passed, setup, teardown),
        cmocka_unit_test_setup_teardown(test_set_state_build_running_after_failed, setup, teardown),
        cmocka_unit_test_setup_teardown(test_set_state_build_running_after_passed, setup, teardown),
        cmocka_unit_test_setup_teardown(test_set_state_build_running_after_off, setup, teardown),
        cmocka_unit_test_setup_teardown(test_set_state_build_running_after_error, setup, teardown),
        cmocka_unit_test_setup_teardown(test_set_state_build_running_after_blink_green, setup, teardown),
        cmocka_unit_test_setup_teardown(test_set_state_build_running_after_blink_red, setup, teardown),
        cmocka_unit_test_setup_teardown(test_signal_error, setup, teardown),
        cmocka_unit_test_setup_teardown(test_lamp_off, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
