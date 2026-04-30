# =============================================================================
# STM32 Blue Pill (STM32F103C8) Bare-Metal CMake
# =============================================================================

cmake_minimum_required(VERSION 3.20)

# -----------------------------------------------------------------------------
# Project
# -----------------------------------------------------------------------------
project(Bluephill_BAREMETAL LANGUAGES C ASM)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# -----------------------------------------------------------------------------
# SAFETY CHECK: forbid MSVC
# -----------------------------------------------------------------------------
if (MSVC)
    message(FATAL_ERROR
        "MSVC detected!\n"
        "Use arm-none-eabi-gcc only."
    )
endif()

# -----------------------------------------------------------------------------
# Language standard
# -----------------------------------------------------------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# MCU FLAGS (STM32F103 = Cortex-M3)
# -----------------------------------------------------------------------------
set(MCU_FLAGS
    -mcpu=cortex-m3
    -mthumb
)

# -----------------------------------------------------------------------------
# Sources
# -----------------------------------------------------------------------------
set(SOURCES
    Src/main.c
    Src/startup_stm32f103xx.S
    Src/uart.c
    Src/adc.c
    Src/systick.c
)

# -----------------------------------------------------------------------------
# Includes
# -----------------------------------------------------------------------------
set(INCLUDES
    Inc
    chip_header/CMSIS/Core/Include
    chip_header/CMSIS/Device/ST/STM32F1xx/Include
)

# -----------------------------------------------------------------------------
# Executable (NO .elf HERE!)
# -----------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# Force ELF suffix
set_target_properties(${PROJECT_NAME}
    PROPERTIES SUFFIX ".elf"
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})

# -----------------------------------------------------------------------------
# Compile options
# -----------------------------------------------------------------------------
target_compile_options(${PROJECT_NAME} PRIVATE
    ${MCU_FLAGS}
    -Wall
    -Wextra
    -Wpedantic
    -O0
    -g3
    -ggdb3
    -gdwarf-2
    -ffunction-sections
    -fdata-sections
    -fno-omit-frame-pointer
)

# -----------------------------------------------------------------------------
# Linker script
# -----------------------------------------------------------------------------
# Prefer a linker script placed in the `linker/` directory, but fall back to
# the project-root `stm32f103x8_flash.ld` if the first one is missing.
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/linker/STM32F103C8Tx_FLASH.ld)

if (NOT EXISTS ${LINKER_SCRIPT})
    set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32f103x8_flash.ld)
endif()

if (NOT EXISTS ${LINKER_SCRIPT})
    message(FATAL_ERROR "Linker script not found: ${LINKER_SCRIPT}")
endif()

target_link_options(${PROJECT_NAME} PRIVATE
    ${MCU_FLAGS}
    -T${LINKER_SCRIPT}
    --specs=nosys.specs
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--print-memory-usage
    -Wl,--cref
)

# Add debug symbols to linker
target_link_options(${PROJECT_NAME} PRIVATE -g3 -ggdb3)

# -----------------------------------------------------------------------------
# Post build: size, hex, bin
# -----------------------------------------------------------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-size $<TARGET_FILE:${PROJECT_NAME}>
    COMMAND arm-none-eabi-objcopy -O ihex
            $<TARGET_FILE:${PROJECT_NAME}>
            ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary
            $<TARGET_FILE:${PROJECT_NAME}>
            ${PROJECT_NAME}.bin
)

# -----------------------------------------------------------------------------
# STM32CubeProgrammer
# -----------------------------------------------------------------------------
find_program(STM32_PROG_CLI STM32_Programmer_CLI)

if (NOT STM32_PROG_CLI)
    message(FATAL_ERROR "STM32_Programmer_CLI not found in PATH")
endif()

# -----------------------------------------------------------------------------
# Custom BUILD target
# -----------------------------------------------------------------------------
add_custom_target(build_fw
    DEPENDS ${PROJECT_NAME}
    COMMENT "Building firmware"
)

# -----------------------------------------------------------------------------
# Custom FLASH target (build + flash)
# -----------------------------------------------------------------------------
add_custom_target(flash_fw
    COMMAND "${STM32_PROG_CLI}"
        -c port=SWD
        -w ${PROJECT_NAME}.bin 0x08000000
        -v
        -rst
    DEPENDS ${PROJECT_NAME}
    COMMENT "Building and flashing firmware"
)

