
#include <stdio.h>              // Biblioteca padrão para entrada e saída (ex: printf)
#include <string.h>             // Biblioteca para manipulação de strings (ex: memset, sprintf)
#include <stdlib.h>             // Biblioteca padrão para funções gerais (ex: conversões)
#include "pico/stdlib.h"        // Biblioteca do SDK do Raspberry Pi Pico para funções básicas (ex: GPIO, sleep)
#include "hardware/adc.h"       // Biblioteca para controle do ADC (Conversor Analógico-Digital)
#include "hardware/i2c.h"       // Biblioteca para comunicação I2C
#include "inc/ssd1306.h"        // Biblioteca para controlar o display OLED SSD1306

// Definição dos pinos conectados ao joystick
#define VRX 26                  // GPIO26 - conectado ao eixo X do joystick (entrada analógica)
#define VRY 27                  // GPIO27 - conectado ao eixo Y do joystick (entrada analógica)
#define SW  22                  // GPIO22 - conectado ao botão do joystick (entrada digital)

// Definição dos pinos usados para I2C (padrão do Pico)
#define I2C_SDA 14               // GPIO2 - SDA do barramento I2C
#define I2C_SCL 15               // GPIO3 - SCL do barramento I2C

int main() {
    stdio_init_all();           // Inicializa a comunicação padrão (ex: para usar printf no terminal via USB)

    adc_init();                 // Inicializa o ADC para leituras analógicas
    adc_gpio_init(VRX);         // Habilita o GPIO26 como entrada analógica (eixo X)
    adc_gpio_init(VRY);         // Habilita o GPIO27 como entrada analógica (eixo Y)

    gpio_init(SW);              // Inicializa o pino do botão como GPIO
    gpio_set_dir(SW, GPIO_IN);  // Define o pino do botão como entrada
    gpio_pull_up(SW);           // Ativa o resistor pull-up interno (estado padrão: HIGH)

    i2c_init(i2c1, 400 * 1000);             // Inicializa o barramento I2C1 a 400kHz (frequência de comunicação)
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Define o pino SDA (GPIO2) para função I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Define o pino SCL (GPIO3) para função I2C
    gpio_pull_up(I2C_SDA);                  // Ativa pull-up para linha SDA (necessário no barramento I2C)
    gpio_pull_up(I2C_SCL);                  // Ativa pull-up para linha SCL (idem)
    
    ssd1306_init();                         // Inicializa o display OLED SSD1306

    // Define a área que será usada para exibir informações no display
    struct render_area area = {
        .start_column = 0,                      // Começa na coluna 0
        .end_column = ssd1306_width - 1,        // Vai até a última coluna do display
        .start_page = 0,                        // Começa na página (linha) 0
        .end_page = ssd1306_n_pages - 1         // Vai até a última página (linha)
    };

    calculate_render_area_buffer_length(&area);    // Calcula o tamanho do buffer necessário para essa área

    uint8_t buffer[ssd1306_buffer_length];         // Cria o buffer que armazenará os dados de exibição do display

    // Início do loop principal do programa
    while (true) {
        adc_select_input(0);           // Seleciona o canal ADC0 (ligado ao eixo X)
        uint16_t x = adc_read();       // Lê o valor digital do eixo X (0 a 4095)

        adc_select_input(1);           // Seleciona o canal ADC1 (ligado ao eixo Y)
        uint16_t y = adc_read();       // Lê o valor digital do eixo Y (0 a 4095)

        bool btn = !gpio_get(SW);      // Lê o estado do botão (inverte pois o botão é pull-up: pressionado = LOW)

        // Exibe os valores no terminal (USB serial)
        printf("Eixo X: %d | Eixo Y: %d | Botão: %s\n", x, y, btn ? "PRESSIONADO" : "SOLTO");

        memset(buffer, 0, sizeof(buffer));    // Limpa o buffer do display, preenchendo com zeros (tela toda apagada)

        char text[32];                        // Buffer de texto para desenhar no display

        sprintf(text, "X: %d", x);            // Formata o texto com o valor de X
        ssd1306_draw_string(buffer, 10, 10, text);  // Escreve no display na posição (10, 10)

        sprintf(text, "Y: %d", y);            // Formata o texto com o valor de Y
        ssd1306_draw_string(buffer, 10, 30, text);  // Escreve no display na posição (10, 30)

        sprintf(text, "BTN: %s", btn ? "ON" : "OFF");  // Formata o texto com o estado do botão
        ssd1306_draw_string(buffer, 10, 50, text);     // Escreve no display na posição (10, 50)

        render_on_display(buffer, &area);     // Envia o buffer para o display, atualizando a tela

        sleep_ms(500);                        // Espera 500 milissegundos antes da próxima leitura
    }

    return 0;  // Nunca será alcançado, mas é boa prática ter um return no main
}
