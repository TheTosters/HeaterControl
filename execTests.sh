#!/bin/bash
#rm -rf build
#mkdir build
cd build
cmake .. -DBUILD_TESTS=ON
make all
./test/time_schedule_tests
