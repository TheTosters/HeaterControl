
find_package(Threads REQUIRED)
#find_package(GTest REQUIRED)
#find_package(GMock REQUIRED)

add_executable(time_schedule_tests
  time_schedule_test.cpp
)

target_link_libraries(time_schedule_tests
    gtest_main)

add_test(NAME time_schedule_tests
    COMMAND ./time_schedule_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)