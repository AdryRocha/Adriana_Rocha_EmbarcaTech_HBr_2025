#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "fonts.h"

// Define o número de níveis (linhas) da Galton Board
#define LEVELS 6

// Define o número de compartimentos na base (sempre LEVELS + 1)
#define BINS (LEVELS + 1)

// Define a largura e altura do display
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

// Struct para representar uma partícula
typedef struct {
    int x;           // Posição X atual da partícula
    int y;           // Posição Y atual da partícula
    int bin_index;   // Índice do compartimento onde caiu
} Particle;

// Struct para representar a Galton Board
typedef struct {
    int bin_counts[BINS];       // Contador de partículas em cada compartimento
    Particle particles[100];    // Armazena partículas simuladas
    int particle_count;         // Quantidade de partículas geradas
} GaltonBoard;

// Função para inicializar o jogo
void init_board(GaltonBoard* board) {
    board->particle_count = 0;  // Zera o contador de partículas
    for (int i = 0; i < BINS; i++) {
        board->bin_counts[i] = 0; // Zera os compartimentos
    }
}

// Simula a queda de uma partícula pela Galton Board
void drop_particle(GaltonBoard* board) {
    int position = 0;
    // Simula os desvios em cada nível (esquerda ou direita)
    for (int i = 0; i < LEVELS; i++) {
        int direction = rand() % 2; // 0 = esquerda, 1 = direita
        if (direction == 1) {
            position++;
        }
    }

    // Atualiza o compartimento final
    board->bin_counts[position]++;
    
    // Cria a partícula e armazena
    Particle p;
    p.x = position;
    p.y = LEVELS;
    p.bin_index = position;
    board->particles[board->particle_count++] = p;
}

// Desenha o histograma no display OLED
void draw_board(GaltonBoard* board) {
    SSD1306_Fill(SSD1306_COLOR_BLACK); // Limpa a tela

    int bar_width = DISPLAY_WIDTH / BINS; // Calcula a largura de cada barra

    for (int i = 0; i < BINS; i++) {
        int height = board->bin_counts[i];
        if (height > 60) height = 60; // Limita a altura para caber no display

        // Desenha retângulo vertical para cada compartimento
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < bar_width - 2; x++) {
                SSD1306_DrawPixel(i * bar_width + x + 1, DISPLAY_HEIGHT - 1 - y, SSD1306_COLOR_WHITE);
            }
        }
    }

    SSD1306_UpdateScreen(); // Atualiza display
}

int main() {
    stdio_init_all();             // Inicializa entrada/saída padrão
    SSD1306_Init();               // Inicializa display
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    SSD1306_UpdateScreen();

    while (true) {
        SSD1306_SetCursor(0, 0);
        SSD1306_DrawString("Galton Board", SSD1306_COLOR_WHITE);
        SSD1306_UpdateScreen();
        sleep_ms(1000);
    }

    GaltonBoard board;
    init_board(&board);           // Inicializa a estrutura da Galton Board
    srand(time(NULL));            // Inicializa o gerador de números aleatórios

    while (1) {
        drop_particle(&board);    // Simula partícula caindo
        draw_board(&board);       // Desenha histograma no display
        sleep_ms(200);            // Aguarda um pouco entre partículas
    }

    return 0;
}
