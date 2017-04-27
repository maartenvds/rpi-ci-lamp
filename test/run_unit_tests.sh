#!/bin/sh

PIDFILE="web_server.pid"

# start test server
python test/tools/web_server.py & echo $! > $PIDFILE

# wait until test server is up and running
until eval "wget -qO- --no-check-certificate https://localhost:4443"
do
    sleep 1
done

# run all tests
./test/test_response_parser &&
    ./test/test_settings_parser &&
    ./test/test_lamp_control_red_green &&
    ./test/test_lamp_io_red_green &&
    ./test/test_system

# stop test server
kill $(cat "$PIDFILE") && rm -f $PIDFILE
