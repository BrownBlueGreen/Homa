# Cross compile settings
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Toolchain
set(TOOLCHAIN_PREFIX arm-none-eabi)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc)

set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}-size)

# Don't try to run executables on the host during compilation
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Common flags
# NOTE: Cortext-M4F typically also needs -mfpu=... if you use FPU instructions
# For stm32f428: MFPU "-mfpu=fvp4-sp-d16"
set(MCPU "-mcpu=cortex-m4")
set(MTHUMB "-mthumb")
set(MFLOAT "-mfloat-abi=hard")
set(MFPU "-mfpu=fpv4-sp-d16")

set(COMMON_FLAGS "${MCPU} ${MTHUMB} ${MFLOAT} ${MFPU}")

set(CMAKE_C_FLAGS_INIT   "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT "${COMMON_FLAGS}")
