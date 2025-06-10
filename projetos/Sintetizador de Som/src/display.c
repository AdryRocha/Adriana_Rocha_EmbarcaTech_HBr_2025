#include "display.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "ssd1306.h"

// --- PINOS I2C PARA A BITDOGLAB v6 ---
#define I2C_PORT  i2c0 // Usaremos a porta I2C0.
#define SDA_PIN   0    // GP0 é o pino SDA no header I2C0.
#define SCL_PIN   1    // GP1 é o pino SCL no header I2C0.
#define OLED_ADDR 0x3C // Endereço I2C padrão para a maioria dos displays SSD1306.

static ssd1306_t oled; // Estrutura que representa nosso display.

void init_display() {
    i2c_init(I2C_PORT, 400 * 1000); // Inicia o I2C0 com velocidade de 400kHz.
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN); // Ativa resistores de pull-up internos.
    gpio_pull_up(SCL_PIN);

    ssd1306_init(&oled, 128, 64, OLED_ADDR, I2C_PORT); // Inicializa o controlador do display.
    ssd1306_clear(&oled); // Limpa o buffer de vídeo.
    ssd1306_show(&oled);  // Envia o buffer limpo para a tela.
}

void clear_display() {
    ssd1306_clear(&oled);
    ssd1306_show(&oled);
}

void show_waveform(uint16_t *buffer, size_t size) {
    ssd1306_clear(&oled);

    // O display tem 128 pixels de largura, então desenhamos 128 amostras.
    size_t max_samples = (size < 128) ? size : 128;

    for (size_t i = 0; i < max_samples; i++) {
        // Converte o valor da amostra (0-4095) para a altura do display (0-63).
        // `>> 6` é uma forma eficiente de dividir por 64.
        uint8_t height = (buffer[i] >> 6);

        // Desenha uma linha vertical para cada amostra.
        ssd1306_draw_line(&oled, i, 63, i, 63 - height);
    }
    ssd1306_show(&oled); // Atualiza a tela com a forma de onda.
}