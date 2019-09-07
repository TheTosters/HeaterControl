PRJ_DIR=$(pwd)
NRF_SDK=${PRJ_DIR}/../nRF5_SDK_15.2.0_9412b96
echo ${NRF_SDK}
if [ ! -d "${NRF_SDK}" ]; then
  echo "Expected folder nRF5_SDK_15.2.0_9412b96 one directory up"
  exit 1
fi

cd ..

if [ ! -d "CMSIS_5-5.6.0" ]; then
  echo "Downloading new CSMI"
  wget https://github.com/ARM-software/CMSIS_5/archive/5.6.0.zip -O cmsi_arch.zip
  unzip -q cmsi_arch.zip
fi

echo "Patching NRF SDK with new CMSI"
yes | cp -f CMSIS_5-5.6.0/CMSIS/DSP/Lib/GCC/libarm_cortexM4lf_math.a ${NRF_SDK}/components/toolchain/cmsis/dsp/GCC
yes | cp -f CMSIS_5-5.6.0/CMSIS/DSP/Lib/GCC/libarm_cortexM4l_math.a ${NRF_SDK}/components/toolchain/cmsis/dsp/GCC
yes | cp -f CMSIS_5-5.6.0/CMSIS/DSP/Lib/ARM/arm_cortexM4lf_math.lib ${NRF_SDK}/components/toolchain/cmsis/dsp/ARM
yes | cp -f CMSIS_5-5.6.0/CMSIS/DSP/Lib/ARM/arm_cortexM4l_math.lib ${NRF_SDK}/components/toolchain/cmsis/dsp/ARM

rm -rf ${NRF_SDK}/components/toolchain/cmsis/include
cp -fr CMSIS_5-5.6.0/CMSIS/Core/Include ${NRF_SDK}/components/toolchain/cmsis/include

cd ${PRJ_DIR}
echo "Prepare local_env.cmake file"

rm -rf local_env.cmake
GCC=$(find ${PRJ_DIR}/.. -name "gcc-arm-none-eabi-*" -type d)
if [ ! -d "${NRF_SDK}" ]; then
  echo "Expected folder with GCC NONE ABI one directory up, please put maually location into local_env.cmake"
  GCC="<Insert path to GCC here>"
fi

NRJ_PROG=${PRJ_DIR}/../nrjprog
if [ ! -d "${NRJ_PROG}" ]; then
  echo "Expected folder with NRJPROG one directory up, please put maually location into local_env.cmake"
  NRJ_PROG="<Insert path to NRJPROG here>"
fi

echo "SET(ARM_NONE_EABI_TOOLCHAIN_PATH \"${GCC}\")" > local_env.cmake
echo "SET(SDK_ROOT \"${NRF_SDK}\")" >> local_env.cmake
echo "SET(NRFJPROG \"${NRJ_PROG}\")" >> local_env.cmake

echo "Setup done Please try building now"
mkdir build
