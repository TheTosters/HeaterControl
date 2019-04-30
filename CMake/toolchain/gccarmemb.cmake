if (CMAKE_HOST_WIN32)
    set(CMAKE_ASM_COMPILER "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc.exe")
    set(CMAKE_C_COMPILER "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc.exe")
    set(CMAKE_CXX_COMPILER "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-g++.exe")
    set(ARM_NONE_EABI_SIZE "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-size.exe")
    set(ARM_NONE_EABI_OBJCOPY "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-objcopy.exe")
else ()
    set(CMAKE_ASM_COMPILER "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc")
    set(CMAKE_C_COMPILER "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc")
    set(CMAKE_CXX_COMPILER "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-g++")
    set(ARM_NONE_EABI_SIZE "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-size")
    set(ARM_NONE_EABI_OBJCOPY "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-objcopy")
endif ()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(data_flags "-ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin --short-enums")
set(warning_flags "-Wall -Wno-attributes -Wno-format -Wno-unused-function")
set(CMAKE_C_FLAGS_INIT "${warning_flags} ${data_flags}")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS_DEBUG          "-O0 -g3"  CACHE STRING "")
set(CMAKE_CXX_FLAGS_DEBUG        "-O0 -g3"  CACHE STRING "")
set(CMAKE_C_FLAGS_MINSIZEREL     "-Os -g "  CACHE STRING "")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O3 -g "  CACHE STRING "")
set(CMAKE_C_FLAGS_RELEASE        "-O3 -DNDEBUG" CACHE STRING "")

set(cortex-m0_DEFINES
    -mcpu=cortex-m0
    -mthumb
    -mabi=aapcs
    -mfloat-abi=soft)

set(cortex-m4_DEFINES
    -mcpu=cortex-m4
    -mthumb
    -mabi=aapcs
    -mfloat-abi=soft)

set(cortex-m4f_DEFINES
    -mcpu=cortex-m4
    -mthumb
    -mabi=aapcs
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16)

function (set_target_link_options target_name linker_file)
    set(link_flags
        ${${ARCH}_DEFINES}
        "-Wl,--gc-sections --specs=nano.specs -L\"${${PLATFORM}_LINK_INCLUDE_DIR}\" \"-L${CMAKE_CURRENT_SOURCE_DIR}/linker\""
        "-Xlinker -Map=\"${CMAKE_CURRENT_BINARY_DIR}/${target_name}.map\""
        "-T\"${linker_file}.ld\"")

    string(REGEX REPLACE ";" " " link_flags "${link_flags}")
    set_target_properties(${target_name} PROPERTIES LINK_FLAGS ${link_flags})
endfunction (set_target_link_options)

function (create_hex executable)
    add_custom_command(
        TARGET ${executable}
        POST_BUILD
        COMMAND ${ARM_NONE_EABI_OBJCOPY} -O ihex ${CMAKE_CURRENT_BINARY_DIR}/${executable}.elf ${CMAKE_CURRENT_BINARY_DIR}/${executable}.hex
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/${executable}.hex)

    add_custom_command(
        TARGET ${executable}
        POST_BUILD
        COMMAND ${ARM_NONE_EABI_SIZE} ${CMAKE_CURRENT_BINARY_DIR}/${executable}.elf)
endfunction(create_hex)


if (CMAKE_INTERPROCEDURAL_OPTIMIZATION)
    message(WARNING
        "CMAKE_INTERPROCEDURAL_OPTIMIZATION enables -flto with GCC which can lead to unexpected behavior. "
        "One particular problem is that the interrupt vector table can be messed up if the startup file "
        "isn't the first source file of the target. In general weak symbols tend to cause problems.\n"
        "More information https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83967. "
        )
endif (CMAKE_INTERPROCEDURAL_OPTIMIZATION)
