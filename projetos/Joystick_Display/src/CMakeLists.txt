# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(joystick_display C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(joystick_display
    joystick_display.c
    inc/ssd1306_i2c.c
)

# Configure o nome e a versão do programa
pico_set_program_name(joystick_display "Joystick_display")
pico_set_program_version(joystick_display "0.1")

# Modifique as configurações de saída UART/USB
pico_enable_stdio_uart(joystick_display 0)
pico_enable_stdio_usb(joystick_display 1)

# Adicione as bibliotecas necessárias
target_link_libraries(joystick_display
    pico_stdlib
    hardware_i2c
    hardware_adc
    hardware_pio
    hardware_spi
)

# Adicione os diretórios de inclusão
target_include_directories(joystick_display PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

# Gere os arquivos binários/mapa/hex
pico_add_extra_outputs(joystick_display)


