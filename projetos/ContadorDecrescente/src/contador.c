#include <stdio.h>                 // Necessário para usar snprintf
#include <string.h>                // Necessário para usar snprintf 
#include "hardware/irq.h"          // Biblioteca para controle de interrupções (GPIO)
#include "pico/stdlib.h"           // Biblioteca padrão do Raspberry Pi Pico (funções básicas de entrada/saída, temporização)
#include "hardware/gpio.h"         // Biblioteca para controle de GPIO (pinos digitais)
#include "hardware/timer.h"        // Biblioteca para controle de temporizadores (sleep_ms)
#include "hardware/i2c.h"          // Biblioteca para controle de I2C (comunicação serial) 
#include "inc/ssd1306.h"           // Biblioteca para controlar o display OLED

// Define GPIO_IRQ_EDGE_FALL manualmente, se necessário
#ifndef GPIO_IRQ_EDGE_FALL
#define GPIO_IRQ_EDGE_FALL 0x02
#endif

#define I2C_PORT i2c0
#define SSD1306_I2C_ADDRESS 0x3C  // endereço padrão mais comum do SSD1306

// Definições de pinos e configurações
#define BUTTON_A 5                // Define o pino GPIO 5 como o botão A (para iniciar a contagem)
#define BUTTON_B 6                // Define o pino GPIO 6 como o botão B (para contar pressões enquanto conta)
#define I2C_PORT i2c0             // Define o barramento I2C 0 como o utilizado para o display OLED
#define SDA_PIN 14                 // Define o pino 14 como SDA (dados) para I2C 
#define SCL_PIN 15                 // Define o pino 15 como SCL (relógio) para I2C

// Variáveis globais para controle do sistema (usadas em interrupções e no loop principal)
volatile int countdown = 0;          // Contador decrescente (de 9 a 0)
volatile int button_b_clicks = 0;    // Contador de cliques do Botão B
volatile bool counting_active = false; // Flag para indicar se a contagem está ativa
volatile bool update_display = true;  // Flag para indicar se o display deve ser atualizado

// Estrutura para área de renderização do display
struct render_area display_area;
uint8_t display_buffer[ssd1306_buffer_length]; // Buffer para armazenar os dados do display
ssd1306_t display; 
//= {SSD1306_I2C_ADDRESS, I2C_PORT, display_buffer, sizeof(display_buffer), &display_area}; // Estrutura para o display OLED


// Função para debounce simples de botão
bool debounce(uint gpio_pin) {
    // Se o botão foi pressionado
    if (gpio_get(gpio_pin) == 0) {  // Assumindo que o botão está com pull-up (0 quando pressionado)
        sleep_ms(20);  // Pequeno delay para eliminação de bouncing
        return (gpio_get(gpio_pin) == 0);  // Retorna true se o botão ainda está pressionado após o delay
    }
    return false;
}

// Manipulador de interrupção do Timer - é chamado a cada segundo durante a contagem
bool timer_callback(struct repeating_timer *t) {
    if (counting_active) {
        if (countdown > 0) {
            countdown--;  // Decrementa o contador
            update_display = true;  // Solicita atualização do display
            
            // Se o contador chegar a zero, desativa a contagem
            if (countdown == 0) {
                counting_active = false;
            }
        }
    }
    return true;  // Continua com o timer
}

// Manipulador genérico de interrupção para os botões A e B
void button_callback(uint gpio, uint32_t events) {
    if (!debounce(gpio)) return;

    if (gpio == BUTTON_A) {
        // Reinicia o contador e zera os cliques do Botão B
        countdown = 9;
        button_b_clicks = 0;
        counting_active = true;  // Ativa a contagem
        update_display = true;   // Solicita atualização do display
    } 
    else if (gpio == BUTTON_B) {
        // Incrementa o contador de cliques apenas se a contagem estiver ativa
        if (counting_active) {
            button_b_clicks++;
            update_display = true;  // Solicita atualização do display
        }
    }

    sleep_ms(200);  // Delay para evitar múltiplos acionamentos
}


// Função para atualizar o display OLED
void update_oled_display() {
    // Limpa o buffer do display
    memset(display_buffer, 0, ssd1306_buffer_length);
    
    // Prepara as strings para exibição
    char countdown_text[32];
    char clicks_text[32];
    char status_text[32];
    
    // Formata a string do contador
    sprintf(countdown_text, "Contador: %d", countdown);
    
    // Formata a string de cliques do Botão B
    sprintf(clicks_text, "Cliques B: %d", button_b_clicks);
    
    // Formata a string de status
    if (counting_active) {
        sprintf(status_text, "Status: CONTANDO");
    } else if (countdown == 0) {
        sprintf(status_text, "Status: FINALIZADO");
    } else {
        sprintf(status_text, "Status: PARADO");
    }
    
    // Desenha os textos no buffer
    ssd1306_draw_string(display_buffer, 5, 10, countdown_text);
    ssd1306_draw_string(display_buffer, 5, 30, clicks_text);
    ssd1306_draw_string(display_buffer, 5, 50, status_text);
    
    // Envia o buffer para o display
    render_on_display(display_buffer, &display_area);
}

int main() {
    // Inicializa todas as interfaces de entrada/saída padrão
    stdio_init_all();
    
    // Aguarda um pouco para garantir que a USB esteja pronta
    sleep_ms(2000);
    
    printf("Iniciando o programa de contador decrescente com interrupção\n");
    
    // Inicializa os pinos dos botões
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);
    
    // Configura os pinos como entrada com pull-up
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);
    
    // Inicializa a comunicação I2C para o display OLED
    i2c_init(I2C_PORT, 400 * 1000);  // Inicializa o barramento I2C com velocidade de 400 kHz
    
    // Configura os pinos para a função I2C
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    
    // Ativa os resistores pull-up internos para a comunicação I2C
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    
    // Inicializa o display OLED
    ssd1306_init_bm(&display, 128, 64, false, 0x3C, I2C_PORT);
    
    // Define a área de renderização no display (toda a tela)
    display_area.start_column = 0;
    display_area.end_column = 127; // 128 pixels de largura
    display_area.start_page = 0;
    display_area.end_page = 7; // 64 pixels de altura (8 páginas de 8 pixels cada)
    
    // Calcula o tamanho necessário do buffer para a área definida
    calculate_render_area_buffer_length(&display_area);
    
    // Configura interrupções para os botões (em modo de borda de descida - quando pressionado)
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);
    
    // Configura um timer para decrementar o contador a cada segundo
    struct repeating_timer timer;
    add_repeating_timer_ms(1000, timer_callback, NULL, &timer);
    
    // Exibe o estado inicial no display
    update_oled_display();
    
    // Loop principal
    while (true) {
        // Verifica se o display precisa ser atualizado
        if (update_display) {
            update_oled_display();
            update_display = false;
        }
        
        // Pequeno delay para não sobrecarregar a CPU
        sleep_ms(10);
    }
    
    return 0;
}
// Fim do código