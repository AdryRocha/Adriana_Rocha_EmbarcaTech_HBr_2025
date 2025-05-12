#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "ssd1306_i2c.h"
#include "galtonboardview.h"

#define BUFFER_LENGTH (SSD1306_WIDTH * SSD1306_HEIGHT / 8) // Tamanho do buffer do display

static uint8_t display_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8]; // Buffer para armazenar os dados do display

void clear_display_buffer() { // Limpa o buffer do display
    memset(display_buffer, 0, BUFFER_LENGTH); // Preenche o buffer com zeros
}

void ssd1306_update_display() { // Atualiza o display com os dados do buffer
    uint8_t command_buffer[2]; // Buffer para comandos
    
    command_buffer[0] = 0x00; // Comando de controle
    command_buffer[1] = 0x21; // Comando para definir a coluna
    i2c_write_blocking(i2c1, 0x3C, command_buffer, 2, false); // Envia o comando
    command_buffer[1] = 0x00; // Coluna inicial
    i2c_write_blocking(i2c1, 0x3C, command_buffer, 2, false); // Envia o comando
    command_buffer[1] = 0x7F; // Coluna final
    i2c_write_blocking(i2c1, 0x3C, command_buffer, 2, false); // Envia o comando
    
    command_buffer[1] = 0x22; // Comando para definir a página
    i2c_write_blocking(i2c1, 0x3C, command_buffer, 2, false); // Envia o comando
    command_buffer[1] = 0x00; // Página inicial
    i2c_write_blocking(i2c1, 0x3C, command_buffer, 2, false); // Envia o comando
    command_buffer[1] = 0x07;   // Página final
    i2c_write_blocking(i2c1, 0x3C, command_buffer, 2, false); // Envia o comando
    
    uint8_t data_buffer[1025]; // Buffer para os dados do display
    data_buffer[0] = 0x40; // Comando de dados
    memcpy(&data_buffer[1], display_buffer, BUFFER_LENGTH); // Copia os dados do buffer para o buffer de dados
    i2c_write_blocking(i2c1, 0x3C, data_buffer, BUFFER_LENGTH + 1, false); // Envia os dados
}

void ssd1306_setup() { // Configura o display com os comandos iniciais
    uint8_t init_commands[] = { // Comandos de inicialização
        0x00, 0xAE, 0x00, 0xD5, 0x80, 0x00, 0xA8, 0x3F, 0x00, 0xD3, 0x00,
        0x00, 0x40, 0x00, 0x8D, 0x14, 0x00, 0x20, 0x00, 0x00, 0xA1, 0x00,
        0xC8, 0x00, 0xDA, 0x12, 0x00, 0x81, 0xCF, 0x00, 0xD9, 0xF1, 0x00,
        0xDB, 0x40, 0x00, 0xA4, 0x00, 0xA6, 0x00, 0xAF
    };
    i2c_write_blocking(i2c1, 0x3C, init_commands, sizeof(init_commands), false); // Envia os comandos de inicialização
}

void init_display() { // Inicializa o display
    i2c_init(i2c1, 400 * 1000); // Inicializa o I2C a 400kHz
    gpio_set_function(14, GPIO_FUNC_I2C); // Configura o pino 14 como I2C
    gpio_set_function(15, GPIO_FUNC_I2C); // Configura o pino 15 como I2C
    gpio_pull_up(14); // Ativa o pull-up no pino 14
    gpio_pull_up(15); // Ativa o pull-up no pino 15
    ssd1306_setup(); // Configura o display
    clear_display_buffer(); // Limpa o buffer do display
    ssd1306_update_display(); // Atualiza o display
    clear_display_buffer(); // Limpa o buffer do display
    ssd1306_update_display(); // Atualiza o display
}

void draw_histogram(int *histogram) { // Desenha o histograma no display
    for (int i = 0; i < CHANNELS; i++) { // Para cada canal
        if (histogram[i] > 0) { // Se houver bolas no canal
            int height = histogram[i] / 2; // Cada 2 bolas adicionam 1 pixel de altura
            if (height > SSD1306_HEIGHT - 10) height = SSD1306_HEIGHT - 10; // Limita a altura
            for (int y = SSD1306_HEIGHT - height; y < SSD1306_HEIGHT; y++) { // Para cada linha de altura
                for (int x = i * CHANNEL_WIDTH; x < (i + 1) * CHANNEL_WIDTH - 1; x++) { // Para cada coluna do canal
                    ssd1306_set_pixel(display_buffer, x, y, true); // Define o pixel como ativo
                }
            }
        }
    }
}

void draw_ball(Ball *ball) { // Desenha uma bola no display
    if (ball->active) { // Se a bola estiver ativa
        ssd1306_set_pixel(display_buffer, (int)ball->x, (int)ball->y, true); // Define o pixel da bola como ativo
    }
}

void draw_probabilities(float left_prob) { // Desenha as probabilidades no display
    char left_buffer[8]; // Buffer para a probabilidade esquerda
    char right_buffer[8]; // Buffer para a probabilidade direita
    snprintf(left_buffer, sizeof(left_buffer), "%.0f%%", left_prob); // Formata a probabilidade esquerda
    snprintf(right_buffer, sizeof(right_buffer), "%.0f%%", 100.0f - left_prob); // Formata a probabilidade direita
    ssd1306_draw_string(display_buffer, 0, 28, left_buffer); // Esquerda, y=28
    ssd1306_draw_string(display_buffer, 104, 28, right_buffer); // Direita, ajustado para caber
}

void update_display(Ball *balls, int ball_count, int *histogram) { // Atualiza o display com as bolas e o histograma
    clear_display_buffer(); // Limpa o buffer do display
    for (int i = 0; i < ball_count; i++) { // Para cada bola
        draw_ball(&balls[i]); // Desenha a bola no display
    }
    draw_histogram(histogram); // Desenha o histograma no display
    char buffer[16]; // Buffer para o texto
    snprintf(buffer, sizeof(buffer), "Bolas: %d", total_balls); // Formata o texto com o número total de bolas
    ssd1306_draw_string(display_buffer, 0, 0, buffer); // Desenha o texto no display
    draw_probabilities(get_left_probability()); // Desenha as probabilidades no display
    ssd1306_update_display(); // Atualiza o display com os dados do buffer
}