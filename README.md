# rpi-ci-lamp

| Metrics         | master        | development   | coverity_scan |
| --------------- |:-------------:|:-------------:|:-------------:|
| Build & test    | [![Travis build](https://travis-ci.org/maartenvds/rpi-ci-lamp.svg?branch=master)](https://travis-ci.org/maartenvds/rpi-ci-lamp) | [![Build Status](https://travis-ci.org/maartenvds/rpi-ci-lamp.svg?branch=development)](https://travis-ci.org/maartenvds/rpi-ci-lamp) | [![Build Status](https://travis-ci.org/maartenvds/rpi-ci-lamp.svg?branch=coverity_scan)](https://travis-ci.org/maartenvds/rpi-ci-lamp) |
| Code coverage   | [![codecov](https://codecov.io/gh/maartenvds/rpi-ci-lamp/branch/master/graph/badge.svg)](https://codecov.io/gh/maartenvds/rpi-ci-lamp/branch/master) | [![codecov](https://codecov.io/gh/maartenvds/rpi-ci-lamp/branch/development/graph/badge.svg)](https://codecov.io/gh/maartenvds/rpi-ci-lamp/branch/development) | [![codecov](https://codecov.io/gh/maartenvds/rpi-ci-lamp/branch/coverity_scan/graph/badge.svg)](https://codecov.io/gh/maartenvds/rpi-ci-lamp/branch/coverity_scan)
| Static analysis | N/A           | N/A           | [![Coverity scan](https://scan.coverity.com/projects/12311/badge.svg)](https://scan.coverity.com/projects/maartenvds-rpi-ci-lamp) |

Note: coverity scan analysis is only ran on the coverity_scan branch to limit the maximum number of allowed builds.

## What is it?
Raspberry pi based CI-lamp to monitor travis-CI builds. The code is still under development.
Currently supports only open source travis builds and a lamp with red/green colours.

## Dependencies

- libcurl4-openssl-dev
- wiringPi
- cmocka (only for testing)
