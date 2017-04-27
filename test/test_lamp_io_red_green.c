#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "lamp_io_red_green.h"
#include <wiringPi.h>

/*
 *  Mocks
 */

void digitalWrite(int pin, int value)
{
    check_expected(pin);
    check_expected(value);
}

void pinMode(int pin, int mode)
{
    (void)pin;
    (void)mode;
}

int wiringPiSetup(void)
{
    return (int)mock();
}

void delay(unsigned int howLong)
{
    (void)howLong;
}

/*
 *  Tests
 */
static void test_lamp_io_init_fails(void **state)
{
    (void)state;
    int res;
    struct LampIoRedGreen lamp_io;

    /* setup */
    will_return(wiringPiSetup, -1);
    /* act */
    res = lamp_io_red_green_init(&lamp_io);
    /* assert */
    assert_int_equal(res, -1);
}

static void test_lamp_io_set_state_off(void **state)
{
    struct LampIoRedGreen *lamp_io = *state;

    /* setup */
    expect_value(digitalWrite, pin, LAMP_IO_RED_PIN);
    expect_value(digitalWrite, value, LOW);
    expect_value(digitalWrite, pin, LAMP_IO_GREEN_PIN);
    expect_value(digitalWrite, value, LOW);
    /* act */
    lamp_io_red_green_set_state(lamp_io, LAMP_STATE_OFF);
}

static void test_lamp_io_set_state_green(void **state)
{
    struct LampIoRedGreen *lamp_io = *state;

    /* setup */
    expect_value(digitalWrite, pin, LAMP_IO_RED_PIN);
    expect_value(digitalWrite, value, LOW);
    expect_value(digitalWrite, pin, LAMP_IO_GREEN_PIN);
    expect_value(digitalWrite, value, HIGH);
    /* act */
    lamp_io_red_green_set_state(lamp_io, LAMP_STATE_GREEN);
}

static void test_lamp_io_set_state_red(void **state)
{
    struct LampIoRedGreen *lamp_io = *state;

    /* setup */
    expect_value(digitalWrite, pin, LAMP_IO_RED_PIN);
    expect_value(digitalWrite, value, HIGH);
    expect_value(digitalWrite, pin, LAMP_IO_GREEN_PIN);
    expect_value(digitalWrite, value, LOW);
    /* act */
    lamp_io_red_green_set_state(lamp_io, LAMP_STATE_RED);
}

static void test_lamp_io_set_state_error(void **state)
{
    struct LampIoRedGreen *lamp_io = *state;

    /* setup */
    expect_value(digitalWrite, pin, LAMP_IO_RED_PIN);
    expect_value(digitalWrite, value, HIGH);
    expect_value(digitalWrite, pin, LAMP_IO_GREEN_PIN);
    expect_value(digitalWrite, value, HIGH);
    /* act */
    lamp_io_red_green_set_state(lamp_io, LAMP_STATE_ERROR);
}

/* To test that the pins are 'blinking', we just test the
 * interal variable 'blinking_lamp' since this is easier
 * than trying to expect multiple calls to 'digitalWrite' from
 * a different thread. */

static void test_lamp_io_set_state_blink_green(void **state)
{
    struct LampIoRedGreen *lamp_io = *state;

    /* setup */
    expect_value(digitalWrite, pin, LAMP_IO_RED_PIN);
    expect_value(digitalWrite, value, LOW);
    /* act */
    lamp_io_red_green_set_state(lamp_io, LAMP_STATE_BLINK_GREEN);
    /* assert */
    assert_int_equal(lamp_io->blinking_lamp, LAMP_IO_GREEN_PIN);
}

static void test_lamp_io_set_state_blink_red(void **state)
{
    struct LampIoRedGreen *lamp_io = *state;

    /* setup */
    expect_value(digitalWrite, pin, LAMP_IO_GREEN_PIN);
    expect_value(digitalWrite, value, LOW);
    /* act */
    lamp_io_red_green_set_state(lamp_io, LAMP_STATE_BLINK_RED);
    /* assert */
    assert_int_equal(lamp_io->blinking_lamp, LAMP_IO_RED_PIN);
}

/*
 * Test fixtures
 */

static int setup(void **state)
{
    int res;
    struct LampIoRedGreen *lamp_io = malloc(sizeof(struct LampIoRedGreen));
    assert_non_null(lamp_io);

    will_return(wiringPiSetup, 0);
    res = lamp_io_red_green_init(lamp_io);
    assert_int_equal(res, 0);
    lamp_io->thread_is_running = 0;     /* do not run blink thread to make testing easier */

    *state = lamp_io;
    return 0;
}

static int teardown(void **state)
{
    lamp_io_red_green_deinit(*state);
    free(*state);
    return 0;
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_lamp_io_init_fails),
        cmocka_unit_test_setup_teardown(test_lamp_io_set_state_off, setup, teardown),
        cmocka_unit_test_setup_teardown(test_lamp_io_set_state_green, setup, teardown),
        cmocka_unit_test_setup_teardown(test_lamp_io_set_state_red, setup, teardown),
        cmocka_unit_test_setup_teardown(test_lamp_io_set_state_error, setup, teardown),
        cmocka_unit_test_setup_teardown(test_lamp_io_set_state_blink_green, setup, teardown),
        cmocka_unit_test_setup_teardown(test_lamp_io_set_state_blink_red, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
