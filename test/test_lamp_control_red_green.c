#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "lamp_control_red_green.h"

/*
 * Mocks
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
    struct LampControlRedGreen *lamp_control = *state;

    /* setup */
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_RED);
    /* act */
    lamp_control_set_state(lamp_control, BUILD_STATE_FAILED);
    /* assert */
    assert_int_equal(lamp_control->lamp_state, LAMP_STATE_RED);
}

static void test_set_state_build_passed(void **state)
{
    struct LampControlRedGreen *lamp_control = *state;

    /* setup */
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_GREEN);
    /* act */
    lamp_control_set_state(lamp_control, BUILD_STATE_PASSED);
    /* assert */
    assert_int_equal(lamp_control->lamp_state, LAMP_STATE_GREEN);
}

static void test_set_state_build_running_after_failed(void **state)
{
    struct LampControlRedGreen *lamp_control = *state;

    /* setup */
    lamp_control->lamp_state = LAMP_STATE_RED;
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_BLINK_RED);
    /* act */
    lamp_control_set_state(lamp_control, BUILD_STATE_RUNNING);
    /* assert */
    assert_int_equal(lamp_control->lamp_state, LAMP_STATE_BLINK_RED);
}

static void test_set_state_build_running_after_passed(void **state)
{
    struct LampControlRedGreen *lamp_control = *state;

    /* setup */
    lamp_control->lamp_state = LAMP_STATE_GREEN;
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_BLINK_GREEN);
    /* act */
    lamp_control_set_state(lamp_control, BUILD_STATE_RUNNING);
    /* assert */
    assert_int_equal(lamp_control->lamp_state, LAMP_STATE_BLINK_GREEN);
}

static void test_set_state_build_running_after_off(void **state)
{
    struct LampControlRedGreen *lamp_control = *state;

    /* setup */
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_BLINK_GREEN);
    /* act */
    lamp_control_set_state(lamp_control, BUILD_STATE_RUNNING);
    /* assert */
    assert_int_equal(lamp_control->lamp_state, LAMP_STATE_BLINK_GREEN);
}

static void test_set_state_build_running_after_error(void **state)
{
    struct LampControlRedGreen *lamp_control = *state;

    /* setup */
    lamp_control->lamp_state = LAMP_STATE_ERROR;
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_BLINK_RED);
    /* act */
    lamp_control_set_state(lamp_control, BUILD_STATE_RUNNING);
    /* assert */
    assert_int_equal(lamp_control->lamp_state, LAMP_STATE_BLINK_RED);
}

static void test_set_state_build_running_after_blink_green(void **state)
{
    struct LampControlRedGreen *lamp_control = *state;

    /* setup */
    lamp_control->lamp_state = LAMP_STATE_BLINK_GREEN;
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_BLINK_GREEN);
    /* act */
    lamp_control_set_state(lamp_control, BUILD_STATE_RUNNING);
    /* assert */
    assert_int_equal(lamp_control->lamp_state, LAMP_STATE_BLINK_GREEN);
}

static void test_set_state_build_running_after_blink_red(void **state)
{
    struct LampControlRedGreen *lamp_control = *state;

    /* setup */
    lamp_control->lamp_state = LAMP_STATE_BLINK_RED;
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_BLINK_RED);
    /* act */
    lamp_control_set_state(lamp_control, BUILD_STATE_RUNNING);
    /* assert */
    assert_int_equal(lamp_control->lamp_state, LAMP_STATE_BLINK_RED);
}

static void test_signal_error(void **state)
{
    struct LampControlRedGreen *lamp_control = *state;

    /* setup */
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_ERROR);
    /* act */
    lamp_control_signal_error(lamp_control);
    /* assert */
    assert_int_equal(lamp_control->lamp_state, LAMP_STATE_ERROR);
}

static void test_lamp_off(void **state)
{
    struct LampControlRedGreen *lamp_control = *state;

    /* setup */
    lamp_control->lamp_state = LAMP_STATE_RED;
    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_OFF);
    /* act */
    lamp_control_off(lamp_control);
    /* assert */
    assert_int_equal(lamp_control->lamp_state, LAMP_STATE_OFF);
}

/*
 * Test fixtures
 */

static int setup(void **state)
{
    struct LampControlRedGreen *lamp_control;

    expect_value(lamp_io_red_green_set_state, lamp_state, LAMP_STATE_OFF);
    lamp_control = lamp_control_init();
    assert_non_null(lamp_control);
    assert_int_equal(lamp_control->lamp_state, LAMP_STATE_OFF);

    *state = lamp_control;

    return 0;
}

static int teardown(void **state)
{
    lamp_control_deinit(*state);
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
