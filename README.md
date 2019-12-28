
# IMPORTANT
Toolchain:
 * gcc-arm-none-eabi-8-2018-q4-major, or
 * gcc-arm-none-eabi-8-2019-q3-update-linux

NRF SDK:
 * nRF5_SDK_15.2.0_9412b96

You also need nrfprog tools and small patch for SDK from CMSIS. But this is handled by ```preapre_env.sh```

# To build & run tests...

This is super-glue solution, so your eyes will bleeed...
It will use host compiler (not cross-compiler) to build host based tests.
It is NOT ment to test all, only most complicated logic. So to have test build and run you have to:
```
cmake .. -DBUILD_TESTS=ON
make all
```

then you can call for example:
```./test/time_schedule_tests```

# Manual patching SDK
If you set C++ to use C++17 dialect then compilation fails :P You have to manualy patch NordicSDK (15.2) and upgrade CMSIS to have things working go here
[Arm CMSIS 5 on Github](https://github.com/ARM-software/CMSIS_5) and download following files which should be placed here:

| CMSIS file |  | Destination |
|------------| - |-------|
|libarm_cortexM4lf_math.a, libarm_cortexM4l_math.a  | > | nordic_SDK_15.2/components/toolchain/cmsis/dsp/GCC|
|arm_cortexM4lf_math.lib  arm_cortexM4l_math.lib | > | nordic_SDK_15.2/components/toolchain/cmsis/dsp/ARM
|CMSIS_5-develop/CMSIS/Core/Include | > |  nordic_SDK_15.2/components/toolchain/cmsis/include |

```clean & build```  all should work

# Variants
This repository contains several firmware variants. It can be used by setting proper defines for ```CMake```, most easy way to do it is to execute ```ccmake```. Below is list of build variants and their descriptions:

| Variant | Entry point | Description |
|-|-|-|
|Indoor SHT_ADV | main_indoor_1.cpp | BT: Advertisement <br> Sensor: SHT30 <br> Buttons: None <br> LED: None <br> Display: None |
|Indoor SHT_GATT | main_indoor_1a.cpp | BT: GATT <br> Sensor: SHT30 <br> Buttons: None <br> LED: None <br> Display: None |
|Indoor DS_FULL | main_indoor_1.cpp | BT: Advertisement <br> Sensor: DS18B20 <br> Buttons: yes <br> LED: None <br> Display: Yes |

# Origins
first instruction taken from:
https://github.com/Polidea/cmake-nRF5x
It is constantly changed and improved (or downproved :P)
