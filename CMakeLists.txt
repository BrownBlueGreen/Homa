cmake_minimum_required(VERSION 4.2)
project(HOMA LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Produce final.elf
add_executable(final.elf
  main.cpp
  Start/startup.cpp
  Start/syscalls.cpp
  Start/sysinit.cpp
  Start/sysmem.cpp
)

# Include dirs for headers 
target_include_directories(final.elf PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Base
  ${CMAKE_CURRENT_SOURCE_DIR}/Core
  ${CMAKE_CURRENT_SOURCE_DIR}/Start
)

# Compile flags (defaults)
target_compile_options(final.elf PRIVATE
  -Wall
  -ffreestanding
  -fno-exceptions
  -fno-rtti
)

# Linker script
target_link_options(final.elf PRIVATE
  "LINKER:-T,${CMAKE_CURRENT_SOURCE_DIR}/linker_script.ld"
  "LINKER:-Map,final.map"
  --specs=nano.specs
)

# Optional but common for bare-metal
# -Wl, --gc-sections reqquires you to also compile with -ffunction-sections -fdata-sections
target_compile_options(final.elf PRIVATE -ffunction-sections -fdata-sections)
target_link_options(final.elf PRIVATE "LINKER:--gc-sections")

# Make a .bin (optional but handy)
add_custom_command(TARGET final.elf POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:final.elf> final.bin
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:final.elf>
)