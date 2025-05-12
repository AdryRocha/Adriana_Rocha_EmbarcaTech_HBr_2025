
#include<stdio.h>
#include<math.h>
#include<time.h>
#include"galtonboard.h"
#include"galtonboardview.h"
#include<stdlib.h>
#include <hardware/gpio.h>
#include <pico/stdlib.h>


// Definições de pinos para os botões
#define BUTTON_A 5
#define BUTTON_B 6
#define DEBOUNCE_MS 200 // Tempo de debounce em milissegundos


int main() { // Inicializa o sistema
    stdio_init_all(); // Inicializa a comunicação serial
    stdio_usb_init(); // Inicializa o USB
    
    sleep_ms(2000); // Tempo para abrir o Monitor Serial
    printf("Iniciando Galton Board...\n"); // Mensagem de inicialização

    gpio_init(BUTTON_A); // Inicializa o botão A
    gpio_init(BUTTON_B); // Inicializa o botão B
    gpio_set_dir(BUTTON_A, GPIO_IN); // Define o botão A como entrada
    gpio_set_dir(BUTTON_B, GPIO_IN); // Define o botão B como entrada
    gpio_pull_up(BUTTON_A); // Ativa o pull-up interno do botão A
    gpio_pull_up(BUTTON_B); // Ativa o pull-up interno do botão B

    init_display(); // Inicializa o display

    Ball balls[MAX_BALLS]; // Array de bolas
    for (int i = 0; i < MAX_BALLS; i++) { // Inicializa as bolas
        balls[i].active = false; // Marca como inativa
    }

    for (int i = 0; i < CHANNELS; i++) { // Inicializa o histograma
        histogram[i] = 0;
    }
    total_balls = 0; // Inicializa o contador de bolas

    extern float left_prob; // Variável de probabilidade
    static uint32_t last_press_a = 0; // Tempo do último pressionamento do botão A
    static uint32_t last_press_b = 0; // Tempo do último pressionamento do botão B
    static bool last_state_a = true; // Último estado do botão A
    static bool last_state_b = true; // Último estado do botão B

    int tick = 0; // Contador de ticks
    while (true) { // Loop principal
        uint32_t now = to_ms_since_boot(get_absolute_time()); // Tempo atual
        bool state_a = gpio_get(BUTTON_A); // Estado atual do botão A
        bool state_b = gpio_get(BUTTON_B); // Estado atual do botão B

        // Detecta borda de descida para botão A (1 -> 0)
        if (!state_a && last_state_a && (now - last_press_a) > DEBOUNCE_MS) { // Se o botão A foi pressionado
            if (left_prob < 80.0f) { // Se a probabilidade for menor que 80%
                left_prob += 10.0f; // Aumenta a probabilidade
            }
            last_press_a = now; // Atualiza o tempo do último pressionamento
        }

        // Detecta borda de descida para botão B (1 -> 0)
        if (!state_b && last_state_b && (now - last_press_b) > DEBOUNCE_MS) {
            if (left_prob > 10.0f) { // Se a probabilidade for maior que 10%
                left_prob -= 10.0f; // Diminui a probabilidade
            }
            last_press_b = now; // Atualiza o tempo do último pressionamento
        }

        last_state_a = state_a; // Atualiza o último estado do botão A
        last_state_b = state_b; // Atualiza o último estado do botão B

        if (tick % 5 == 0) { // A cada 5 ticks, cria uma nova bola
            for (int i = 0; i < MAX_BALLS; i++) { // Verifica se há espaço para uma nova bola
                if (!balls[i].active) { // Se a bola não estiver ativa
                    init_ball(&balls[i]); // Inicializa a nova bola
                    break; // Sai do loop
                }
            }
        }

        for (int i = 0; i < MAX_BALLS; i++) { // Atualiza as bolas
            if (balls[i].active) { // Se a bola estiver ativa
                update_ball(&balls[i]); // Atualiza a posição da bola
                if (!balls[i].active) { // Se a bola não estiver mais ativa
                    register_ball_landing(&balls[i]); // Registra a queda da bola
                    if (total_balls % 100 == 0 && total_balls > 0) { // A cada 100 bolas, calcula as estatísticas
                        calculate_statistics();
                    }
                }
            }
        }

        for (int i = 0; i < CHANNELS; i++) { // Limpa o histograma
            if (histogram[i] < 0) histogram[i] = 0; // Se o valor for negativo, zera
        }
        update_display(balls, MAX_BALLS, histogram); // Atualiza o display com as bolas e o histograma

        tick++; // Incrementa o contador de ticks
        sleep_ms(100); // Espera 100ms
    }

    return 0;
}