#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "galtonboard.h"

int histogram[CHANNELS] = {0}; // Histograma para armazenar o número de bolas em cada canal
int total_balls = 0; // Contador total de bolas
float left_prob = 50.0f; // Probabilidade inicial: 50% esquerda

// Implementação da função para obter um número aleatório
uint32_t get_random_32(void) {
    return time_us_32(); // Usa o temporizador de microssegundos como fonte de aleatoriedade
}

// Função para determinar a direção aleatória
bool random_direction(void) {
    return (get_random_32() % 100) < left_prob; // Ex.: 60% esquerda -> true se rand < 60
}

void test_randomness(int trials) { // Função para testar a aleatoriedade
    int left = 0, right = 0; // Contadores para esquerda e direita
    for (int i = 0; i < trials; i++) { // Realiza o teste
        if (random_direction() == 0) right++; // Se a direção for esquerda
        else left++; // Se a direção for direita
    }
    printf("Esquerda: %d (%.2f%%), Direita: %d (%.2f%%)\n", // Exibe os resultados
            left, (float)left / trials * 100, // Porcentagem esquerda
            right, (float)right / trials * 100); // Porcentagem direita
}

void calculate_statistics() { // Função para calcular estatísticas do histograma
    if (total_balls == 0) { // Se não houver bolas registradas
        printf("Nenhuma bola registrada.\n"); // Exibe mensagem
        return;
    }

    float mean = 0.0f; // Média
    for (int i = 0; i < CHANNELS; i++) { // Calcula a média
        mean += (i + 1) * histogram[i]; // Multiplica o índice pelo número de bolas
    }
    mean /= total_balls; // Divide pela quantidade total de bolas   

    float variance = 0.0f; // Variância
    for (int i = 0; i < CHANNELS; i++) { // Calcula a variância
        variance += histogram[i] * ((i + 1) - mean) * ((i + 1) - mean); // (i + 1) é o índice do canal
    }
    variance /= total_balls; // Divide pela quantidade total de bolas
    float std_dev = sqrtf(variance); // Desvio padrão
    printf("Estatísticas:\n"); // Exibe estatísticas
    printf("Média: %.2f\n", mean); // Exibe média
    printf("Desvio Padrão: %.2f\n", std_dev); // Exibe desvio padrão
    printf("Histograma:\n"); // Exibe histograma

    printf("Total de Bolas: %d\n", total_balls); // Exibe total de bolas
    printf("Bins: "); // Exibe bins
    for (int i = 0; i < CHANNELS; i++) { // Exibe cada canal
        printf("%d ", histogram[i]); // Exibe o número de bolas em cada canal
    }
    printf("\n"); // Nova linha
    printf("\nMédia: %.2f\nDesvio Padrão: %.2f\n", mean, std_dev); // Exibe média e desvio padrão
}

void init_ball(Ball *ball) { // Função para inicializar uma bola
    ball->x = SSD1306_WIDTH / 2.0f; // Posição inicial no meio da tela
    ball->y = 0.0f; // Posição inicial no topo da tela
    ball->active = true; // Marca a bola como ativa
    ball->collisions = 0; // Inicializa o número de colisões
}

void update_ball(Ball *ball) { // Função para atualizar a posição da bola
    if (!ball->active) return; // Se a bola não estiver ativa, não faz nada

    ball->y += 1.0f; // Move a bola para baixo
    if (ball->collisions < 15 && ball->y >= (ball->collisions + 1) * (SSD1306_HEIGHT / 15.0f)) { // Se a bola ainda não atingiu o chão
        bool dir = random_direction(); // Determina a direção aleatória
        if (dir) { // Se a direção for esquerda
            ball->x -= 4.0f; // Esquerda
        } else { // Se a direção for direita
            ball->x += 4.0f; // Direita
        }
        ball->collisions++; // Incrementa o número de colisões
    }

    if (ball->x < 0) ball->x = 0; // Se a bola sair pela esquerda, ajusta a posição
    else if (ball->x >= SSD1306_WIDTH) ball->x = SSD1306_WIDTH - 1; // Se a bola sair pela direita, ajusta a posição
    if (ball->x >= SSD1306_WIDTH) ball->x = SSD1306_WIDTH - 1; // Se a bola sair pela direita, ajusta a posição
    if (ball->y >= SSD1306_HEIGHT) { // Se a bola atingir o chão
        ball->active = false; // Marca a bola como inativa
    }
}

void register_ball_landing(Ball *ball) { // Função para registrar a queda da bola
    int bin = (int)(ball->x / (SSD1306_WIDTH / CHANNELS)); // Calcula o canal correspondente
    if (bin >= 0 && bin < CHANNELS) { // Se o canal estiver dentro do intervalo
        histogram[bin]++; // Incrementa o contador do canal
        total_balls++; // Incrementa o contador total de bolas
    }
}

float get_left_probability() { // Função para obter a probabilidade de uma bola cair à esquerda
    return left_prob; // Retorna a probabilidade atual
}

float get_right_probability() { // Função para obter a probabilidade de uma bola cair à direita
    return 100.0f - left_prob; // Retorna a probabilidade atual
}