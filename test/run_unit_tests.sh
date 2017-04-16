#!/bin/sh

python test/tools/web_server.py &
sleep 10
./test/test_response_parser &&
    ./test/test_settings_parser &&
    ./test/test_lamp_control_red_green &&
    ./test/test_application &&
    ./test/test_system
