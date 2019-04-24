cmake_minimum_required(VERSION 3.6)

set(CMAKE_SYSTEM_NAME "Generic")

project(home-multi-temp LANGUAGES NONE)

set(NRF_TARGET "nrf52")

#use this for production board -> production_board.h 
#set(CUSTOM_BOARD_INC "production_board")
#use this for development board -> dev_board.h
set(CUSTOM_BOARD_INC "dev_board")

if (BUILD_TESTS)
#Tests support
  enable_language(C CXX ASM)
  set(CMAKE_C_STANDARD 99)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_FLAGS "-Og -g3 -fsanitize=address  -static-libasan")
  include_directories("include")
  
  configure_file(cmake/gtest-download.cmake.in googletest-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
  if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
  if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
  endif()
  
  # Prevent overriding the parent project's compiler/linker
  # settings on Windows
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  
  # Add googletest directly to our build. This defines
  # the gtest and gtest_main targets.
  add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                   ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                   EXCLUDE_FROM_ALL)
  
  # The gtest/gtest_main targets carry header search path
  # dependencies automatically when using CMake 2.8.11 or
  # later. Otherwise we have to add them here ourselves.
  if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
  endif()

  add_subdirectory(test)
  enable_testing()
else()
  include("local_env.cmake")
  include("cmake/CMake_nRF5x.cmake")
  include("cmake/SoftDevice.cmake")
  include("fix.cmake")

  nRF5x_setup()
  nRF5x_setup_softdevice(s140)
  
  enable_language(C CXX ASM)
  
  include_directories(${NRF5_SDK_PATH}/config/nrf52840/config)
  include_directories("include")
  include_directories(${SDK_SEARCH_DIRECTORIES})
  include_directories(${NRF5_SDK_PATH}/arm-none-eabi/include)
  
  list(APPEND SOURCE_FILES 
    "src/main.cpp"
    "src/ssd1306.cpp"
    ${EXTRA_SDK_SOURCE_FILES}
  ) 
  
  nRF5x_addExecutable(${PROJECT_NAME} "${SOURCE_FILES}")  
endif()



