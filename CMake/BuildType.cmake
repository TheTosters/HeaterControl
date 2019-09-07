# From https://blog.kitware.com/cmake-and-the-default-build-type/

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if (BUILD_HOST AND NOT CMAKE_BUILD_TYPE MATCHES "Debug")
    message(WARNING
        "You are building for host with \"${CMAKE_BUILD_TYPE}\". "
        "You should consider using Debug.")
endif ()

message(STATUS "Setting build type to '${default_build_type}'.")