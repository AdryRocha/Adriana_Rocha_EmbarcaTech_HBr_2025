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
set(sdkVersion 1.5.1)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Define a placa (Pico W neste caso)
set(PICO_BOARD pico_w)

# Defina o caminho para o SDK do Raspberry Pi Pico
set(PICO_SDK_PATH "C:/Users/user/.pico-sdk/sdk/2.1.1")

# Pull in Raspberry Pi Pico SDK (must be before project)
#include(pico_sdk_import.cmake)
include("C:/Users/User/.pico-sdk/sdk/2.1.1/external/pico_sdk_import.cmake")

project(galtonboard C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Gera o compile_commands.json para análise de código

# Inicializa o SDK da Pico
pico_sdk_init()

# Adiciona os diretórios de inclusão
include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# Add executable. Default name is the project name, version 0.1

add_executable(galtonboard include/ssd1306_i2c.c src/main.c src/galtonboard.c src/galtonboardview.c) 

pico_set_program_name(galtonboard "galtonboard")
pico_set_program_version(galtonboard "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(galtonboard 0)
pico_enable_stdio_usb(galtonboard 1)

# Add the standard library to the build
target_link_libraries(galtonboard
        pico_stdlib
        hardware_i2c
        )

# Add the standard include files to the build
target_include_directories(galtonboard PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src
  ${CMAKE_CURRENT_LIST_DIR}/include
)

# Add any user requested libraries
target_link_libraries(galtonboard
        hardware_i2c
        pico_stdlib
        )
pico_add_extra_outputs(galtonboard)
