#!/bin/sh

./test/test_response_parser &&
    ./test/test_settings_parser &&
    ./test/test_lamp_control_red_green &&
    ./test/test_application
