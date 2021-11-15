# rpi-ci-lamp

| Metrics         | master        | development   |
| --------------- |:-------------:|:-------------:|
| Build & test    | [![Build status](https://circleci.com/gh/maartenvds/rpi-ci-lamp/tree/master.svg?style=svg)](https://circleci.com/gh/maartenvds/rpi-ci-lamp/tree/master) | [![Build Status](https://circleci.com/gh/maartenvds/rpi-ci-lamp/tree/development.svg?style=svg)](https://circleci.com/gh/maartenvds/rpi-ci-lamp/tree/development) |
| Code coverage   | [![codecov](https://app.codecov.io/gh/maartenvds/rpi-ci-lamp/branch/master/graph/badge.svg)](https://codecov.io/gh/maartenvds/rpi-ci-lamp/branch/master) | [![codecov](https://codecov.io/gh/maartenvds/rpi-ci-lamp/branch/development/graph/badge.svg)](https://app.codecov.io/gh/maartenvds/rpi-ci-lamp/branch/development) |


Static analysis [![Coverity scan](https://scan.coverity.com/projects/12311/badge.svg)](https://scan.coverity.com/projects/maartenvds-rpi-ci-lamp)

## What is it?
Raspberry pi based CI-lamp to monitor CI-builds. Any HTTPS/REST based CI-API will do.
Examples available for:
* Travis-CI
* Gitlab pipelines

Currently supported lamps:
* red/green signal tower

## Compiling

Dependencies for the build:
- libcurl4-openssl-dev
- libconf-dev
- wiringPi
- cmocka (only for running unittests)
- libasan0 (only when compiling in debug mode)

On a raspberrypi (tested on raspberrypi3 raspbian image)

    make
  
The I/O pin configuration follows the wiringPi enumeration (https://de.pinout.xyz/pinout/wiringpi# -> the numbers between round brackets). By default:
* red lamp = pin 23
* green lamp = pin 21

You can override the default pins:

    export CFLAGS="-DLAMP_IO_RED_PIN=<PIN_NUMBER_RED_LAMP> -DLAMP_IO_GREEN_PIN=<PIN_NUMBER_GREEN_LAMP>"
    make
  
Compiling on a desktop machine

    make NOWIRINGPI=1
  
You can run the application, but of course you will get no I/O feedback. To print the red/green status on stdout for testing, compile in debug mode:

    make DEBUG=1 NOWIRINGPI=1
    
## Running

To quickly test the lamp, you can use the config file that monitors this project on the travis-CI:

    ./bin/rpi-ci-lamp config_examples/rpi_ci_lamp.conf

## Configuration

You will of course want to create your own configuration file. A basic config file would look like:
  
    interval = 5;   # delay in seconds between two consecutive query sessions
    build-info = (
        {
            server = "my_server";
            url = "https://my_servers_api.org/some/path/to/repo/branch";
        }
    );

    my_server = {
        headers = ("my_header: content");   # optional list of HTTP headers
        # regular expressions to parse the response from the CI-server
        regex_passed = "\\\"status\\\":\\\"success\\\"";    # needs to match when build passed
        regex_running = "\\\"status\\\":\\\"running\\\"";   # needs to match when build is running/busy
        regex_failed = "\\\"status\\\":\\\"failed\\\"";     # needs to match when build failed
    };
    
Note that you can add multiple queries and multiple server configs. See the config_examples dir for specific examples.

## Running as a service

There is also a service script in the 'bin' folder that can be used to run the application as a system service so it will start at boot:

    sudo cp bin/rpi-ci-lamp.sh /etc/init.d/
    # Adapt BASEDIR and SCRIPT to your environment. In SCRIPT you also mension your own config file
    sudo vi /etc/init.d/rpi-ci-lamp.sh
    sudo update-rc.d rpi-ci-lamp.sh defaults
    sudo service rpi-ci-lamp start
    
