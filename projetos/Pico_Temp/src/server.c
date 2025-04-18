// =====================================================================================
// Projeto: Monitor de Temperatura Interna do Raspberry Pi Pico com Display OLED SSD1306
// Autora: Adriana Rocha
// Data: 16/04/2025
// Descrição: Lê a temperatura interna do RP2040 e exibe no terminal e display OLED I2C
// =====================================================================================

// --------------------- Inclusão de bibliotecas padrão do Pico e periféricos ---------------------
#include <stdio.h>              // Entrada/saída padrão (ex: printf)
#include <string.h>             // Manipulação de strings (ex: memset, sprintf)
#include "pico/stdlib.h"        // Funções básicas do Raspberry Pi Pico
#include "hardware/adc.h"       // Controle do ADC (conversor analógico-digital)
#include "hardware/i2c.h"       // Comunicação I2C
#include "hardware/gpio.h"      // Controle de GPIOs (pinos digitais)
#include "hardware/timer.h"     // Controle de temporizadores
#include "ssd1306.h"            // Controle do display OLED SSD1306 (custom lib)

// ------------------------ Definições de pinos do display OLED I2C -------------------------------
#define I2C_SDA 14              // GPIO 14 será usado como SDA (dados I2C)
#define I2C_SCL 15              // GPIO 15 será usado como SCL (clock I2C)

// ---------------------- Definições relacionadas ao sensor de temperatura -------------------------
#define TEMP_ADC_CHANNEL 4      // Canal 4 do ADC é ligado ao sensor de temperatura interno

// Constantes para conversão de leitura ADC em temperatura (valores baseados no datasheet do RP2040)
const float CONVERSION_FACTOR = 3.3f / (1 << 12);    // 3.3V dividido por 4096 níveis do ADC
#define TEMP_REFERENCE        27.0f                  // Temperatura de referência (graus Celsius)
#define TEMP_COEFFICIENT      0.001721f              // Coeficiente térmico (volts por grau)
#define VOLTAGE_AT_REFERENCE  0.706f                 // Voltagem esperada a 27°C

// ---------------------- Função para converter valor ADC em temperatura em Celsius ----------------
float convert_adc_to_temp(uint16_t adc_value) {
    float voltage = adc_value * CONVERSION_FACTOR;                         // Converte ADC para tensão
    float temperature = TEMP_REFERENCE - (voltage - VOLTAGE_AT_REFERENCE) / TEMP_COEFFICIENT;
    return temperature;                                                    // Retorna a temperatura
}

// -------------------- Função que retorna o tamanho do buffer para renderização ------------------
size_t calculate_render_area_buffer_length(struct render_area *area) {
    return (area->end_column - area->start_column + 1) *
           (area->end_page - area->start_page + 1); // Calcula o tamanho do buffer
}   
// -------------------------------------- Função principal -----------------------------------------
int main() {
    stdio_init_all();                  // Inicializa UART/USB para comunicação via terminal
    sleep_ms(500);                     // Aguarda 500 milissegundos (garante que USB esteja pronta)

    printf("Iniciando leitura de temperatura interna do RP2040...\n");

    // ------------------------- Inicialização do ADC e sensor interno ------------------------------
    adc_init();                        // Inicializa sistema ADC
    adc_set_temp_sensor_enabled(true); // Ativa o sensor de temperatura embutido

    // ------------------------- Inicialização do barramento I2C para o display ---------------------
    i2c_init(i2c1, 400 * 1000);        // Inicializa o I2C1 a 400 kHz (frequência comum para SSD1306)
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);   // Configura GPIO 14 como SDA
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);   // Configura GPIO 15 como SCL
    gpio_pull_up(I2C_SDA);                      // Habilita resistor de pull-up interno no SDA
    gpio_pull_up(I2C_SCL);                      // Habilita resistor de pull-up interno no SCL

    // ------------------------ Inicialização do display OLED SSD1306 -------------------------------
    ssd1306_init();  // Inicializa display (endereço padrão 0x3C)

    // Define a área do display onde os dados serão desenhados
    struct render_area area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,       // 128 pixels de largura (0-127)
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1        // 8 páginas de 8 pixels (0-7) = 64 pixels de altura
    };

    // Calcula o tamanho necessário do buffer e aloca
    area.buffer_length = calculate_render_area_buffer_length(&area);
    uint8_t buffer[area.buffer_length];

    // -------------------- Mostra mensagem inicial no display --------------------------------------
    memset(buffer, 0, sizeof(buffer));
    ssd1306_draw_string(buffer, 10, 20, "Iniciando...");
    render_on_display(buffer, &area);
    sleep_ms(2000);  // Aguarda 2 segundos (permite inicialização e leitura)    

    // -------------------------------- Loop principal do programa ----------------------------------
    while (true) {
        adc_select_input(TEMP_ADC_CHANNEL);       // Seleciona canal 4 (sensor interno)
        uint16_t raw_temp = adc_read();           // Lê valor bruto do ADC (0 a 4095)
        float temperature = convert_adc_to_temp(raw_temp);  // Converte valor para temperatura em °C

        memset(buffer, 0, sizeof(buffer));        // Limpa o buffer (zera todos os pixels)

        char temp_text[32];
        sprintf(temp_text, "Temperatura:");                  // Primeira linha
        ssd1306_draw_string(buffer, 10, 10, temp_text);

        sprintf(temp_text, "%.2f C", temperature);           // Segunda linha (valor em graus Celsius)
        ssd1306_draw_string(buffer, 10, 30, temp_text);

        sprintf(temp_text, "ADC: %d", raw_temp);             // Terceira linha (valor do ADC bruto)
        ssd1306_draw_string(buffer, 10, 50, temp_text);

        render_on_display(buffer, &area);                    // Atualiza display com novo conteúdo

        printf("Temperatura: %.2f °C (ADC: %d)\n", temperature, raw_temp);  // Também imprime no terminal USB

        sleep_ms(1000);  // Aguarda 1 segundo antes da próxima leitura
    }

    return 0;
}
// Fim do arquivo server.c
// =====================================================================================