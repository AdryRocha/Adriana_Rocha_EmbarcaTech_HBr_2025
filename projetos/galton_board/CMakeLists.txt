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

cmake_minimum_required(VERSION 3.13)

# Define a placa (Pico W neste caso)
set(PICO_BOARD pico_w)

# Defina o caminho para o SDK do Raspberry Pi Pico
set(PICO_SDK_PATH "C:/Users/user/.pico-sdk/sdk/2.1.1")

# Pull in Raspberry Pi Pico SDK (must be before project)
#include(pico_sdk_import.cmake)
include("C:/Users/User/.pico-sdk/sdk/2.1.1/external/pico_sdk_import.cmake")

# Define o nome do projeto
project(Galton_Board C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Gera o compile_commands.json para análise de código

# Remove the second project definition
# project(ssd1306_i2c C CXX ASM)

# Inicializa o SDK da Pico
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Galton_Board
        main.c 
        ssd1306.c
        fonts.c 
        )

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Galton_Board 0)
pico_enable_stdio_usb(Galton_Board 0)

# pull in common dependencies and additional i2c hardware support
target_link_libraries(Galton_Board pico_stdio pico_stdlib hardware_i2c)
target_include_directories(Galton_Board PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# create map/bin/hex file etc.
pico_add_extra_outputs(Galton_Board ssd1306_i2c)

# add url via pico_set_program_url
