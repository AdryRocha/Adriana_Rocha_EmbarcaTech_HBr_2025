# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Defina explicitamente o caminho do SDK
set(PICO_SDK_PATH "C:/Users/User/.pico-sdk/sdk/2.1.1")

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

project(picow_ble_temp_sensor C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

set(WIFI_SSID "Your Wi-Fi SSID")
set(WIFI_PASSWORD "Your Wi-Fi Password")

# Add executable. Default name is the project name, version 0.1
add_executable(picow_ble_temp_sensor
    server.c ssd1306.h
    ssd1306_i2c.c
    ssd1306.c
    fonts.c
    )
target_link_libraries(picow_ble_temp_sensor
    pico_stdlib
    hardware_adc
    )
target_include_directories(picow_ble_temp_sensor PRIVATE
    ${CMAKE_CURRENT_LIST_DIR} # For btstack config
    )
pico_btstack_make_gatt_header(picow_ble_temp_sensor PRIVATE "${CMAKE_CURRENT_LIST_DIR}/temp_sensor.gatt")

pico_add_extra_outputs(picow_ble_temp_sensor)

# Standalone example that connects to picow_ble_temp_sensor and reads the temperature 
# Flahes once quickly each second when it's running but not connected to another device
# Flashes twice quickly each second when connected to another device and reading it's temperature
add_executable(picow_ble_temp_reader
    server.c 
    ssd1306.h
    fonts.c
    )

# Habilita a saída UART padrão
# Habilita a saída UART e USB padrão
pico_enable_stdio_uart(picow_ble_temp_reader 1)
pico_enable_stdio_usb(picow_ble_temp_reader 1)


    target_link_libraries(picow_ble_temp_reader
    pico_stdlib
    hardware_i2c
    pico_btstack_ble
    pico_cyw43_arch_none
    )
    
# Habilita a saída USB padrão
pico_enable_stdio_usb(picow_ble_temp_reader 1) 
pico_enable_stdio_uart(picow_ble_temp_reader 0)

target_include_directories(picow_ble_temp_reader PRIVATE
    ${CMAKE_CURRENT_LIST_DIR} # For btstack config
    )
target_compile_definitions(picow_ble_temp_reader PRIVATE
    RUNNING_AS_CLIENT=1
)

if (WIFI_SSID AND WIFI_PASSWORD)
    # Another version of the sensor example, but this time also runs iperf over wifi
    add_executable(picow_ble_temp_sensor_with_wifi
        server_with_wifi.c # server_common.c
        )
         
        target_include_directories(picow_ble_temp_sensor_with_wifi PRIVATE
        ${CMAKE_CURRENT_LIST_DIR} # For btstack config
        )
    target_compile_definitions(picow_ble_temp_sensor_with_wifi PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        )
    pico_btstack_make_gatt_header(picow_ble_temp_sensor_with_wifi PRIVATE "${CMAKE_CURRENT_LIST_DIR}/temp_sensor.gatt")

    # Habilita a saída USB padrão
    pico_add_extra_outputs(picow_ble_temp_reader)
endif()