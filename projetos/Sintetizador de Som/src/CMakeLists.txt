# Versão mínima do CMake para o projeto.
cmake_minimum_required(VERSION 3.13)

# Define os padrões das linguagens C e C++.
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
# == END OF DO NOT EDIT LINES ==

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Inclui as configurações do SDK do Pico.
include(pico_sdk_import.cmake)

# Define a placa alvo. Como você tem uma Pico W, usamos "pico_w".
set(PICO_BOARD pico_w)

# Define o nome do projeto.
project(sintetizador_audio C CXX ASM)

# Inicializa o SDK, preparando as configurações para a placa.
pico_sdk_init()

# Adiciona o executável final do projeto.
# Inclui todos os nossos arquivos .c e também o .c da biblioteca do display.
add_executable(sintetizador_audio
        main.c
        audio.c
        display.c
        utils.c
        lib/pico-ssd1306/ssd1306.c
        )

pico_set_program_name(sintetizador_audio "sintetizador_audio")
pico_set_program_version(sintetizador_audio "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(sintetizador_audio 0)
pico_enable_stdio_usb(sintetizador_audio 1)

# Adiciona os diretórios onde o compilador deve procurar os arquivos de cabeçalho (.h).
# Isso permite que usemos #include "ssd1306.h" nos nossos arquivos.
target_include_directories(sintetizador_audio PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/pico-ssd1306
)

# Vincula as bibliotecas de hardware do SDK do Pico ao nosso executável.
# - pico_stdlib: Funções padrão (GPIO, tempo, etc.).
# - hardware_adc: Para usar o Conversor Analógico-Digital (gravação).
# - hardware_pwm: Para usar a Modulação por Largura de Pulso (reprodução).
# - hardware_i2c: Para a comunicação com o display OLED.
target_link_libraries(sintetizador_audio
        pico_stdlib
        hardware_adc
        hardware_pwm
        hardware_i2c
        )

# Gera os arquivos de saída finais, incluindo o .uf2 que será gravado na Pico.
pico_add_extra_outputs(sintetizador_audio)