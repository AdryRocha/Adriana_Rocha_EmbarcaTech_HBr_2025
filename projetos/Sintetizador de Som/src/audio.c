/**
 * @file audio.c
 * @brief Módulo para captura e reprodução de áudio.
 * * Este arquivo contém todas as funções necessárias para interagir com o microfone
 * (usando o ADC) e com o buzzer (usando PWM), implementando a lógica de gravação
 * e reprodução do áudio.
 */

#include "audio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <stdio.h>

// --- PINOS E CONFIGURAÇÕES DE ÁUDIO ---
// Define o pino de saída para o buzzer, que será controlado por PWM.
#define PWM_AUDIO_OUT_PIN 21 // 

// Define o pino de entrada para o microfone, que é uma entrada analógica.
#define ADC_MIC_IN_PIN 26 // 

// Define o canal do ADC a ser usado (GP26 corresponde ao ADC0).
#define ADC_INPUT 0 // 

// Define a Taxa de Amostragem em Hertz (Hz).
// 8000 Hz é uma taxa comum para gravação de voz, capturando frequências
// de até 4kHz (pelo Teorema de Nyquist), o que é suficiente para a fala humana.
#define SAMPLE_RATE 8000 // 

/**
 * @brief Inicializa o hardware do Conversor Analógico-Digital (ADC).
 * * Configura o pino do microfone para receber um sinal analógico e prepara
 * o periférico ADC para a leitura.
 */
void init_adc() {
    // Inicializa o periférico ADC do RP2040.
    adc_init();
    
    // Conecta o pino físico (GP26) à função de entrada do ADC.
    adc_gpio_init(ADC_MIC_IN_PIN);
    
    // Seleciona qual canal do ADC será lido (no caso, o canal 0).
    adc_select_input(ADC_INPUT);
}

/**
 * @brief Inicializa o hardware de Modulação por Largura de Pulso (PWM).
 * * Configura o pino do buzzer para gerar o sinal PWM que irá reproduzir o som.
 * As configurações de clock e wrap foram ajustadas para otimizar a qualidade
 * do áudio em um buzzer.
 */
void init_pwm() {
    // Conecta o pino físico (GP21) à função de saída do PWM.
    gpio_set_function(PWM_AUDIO_OUT_PIN, GPIO_FUNC_PWM); // 
    
    // O hardware PWM do Pico é dividido em "slices". Esta linha descobre
    // qual slice está conectado ao nosso pino de saída.
    uint slice = pwm_gpio_to_slice_num(PWM_AUDIO_OUT_PIN); // 
    
    // Pega uma estrutura de configuração com valores padrão.
    pwm_config config = pwm_get_default_config();
    
    // --- ALTERAÇÃO 1: Divisor de Clock ---
    // Define o divisor do clock do sistema para o PWM. 
    // Usar 2.0f faz o contador PWM rodar mais rápido, o que ajuda a mover a
    // frequência da portadora PWM para uma faixa ultrassônica (inaudível).
    pwm_config_set_clkdiv(&config, 2.0f); // 

    // --- ALTERAÇÃO 2: Valor de Wrap ---
    // Define o valor máximo que o contador do PWM alcançará antes de zerar.
    // Este valor determina a FREQUÊNCIA da portadora e a RESOLUÇÃO do áudio.
    // Com Wrap = 250, temos 251 níveis de duty-cycle (~8 bits de resolução).
    // A frequência da portadora será: 125MHz / (1.0 * (250 + 1)) ≈ 498 kHz.
    pwm_config_set_wrap(&config, 250); // 
    
    // Aplica a configuração modificada e habilita o slice do PWM.
    pwm_init(slice, &config, true);
}

/**
 * @brief Captura um número 'size' de amostras de áudio do microfone.
 * * @param buffer Ponteiro para o array onde as amostras serão armazenadas.
 * @param size O número total de amostras a serem capturadas.
 */
void capture_audio(uint16_t *buffer, size_t size) {
    // MENSAGEM 1: Avisa que a função começou.
    printf("DEBUG: Entrou em capture_audio. Tentando capturar %u amostras.\n", size);
    // Loop para capturar cada uma das amostras.
    for (size_t i = 0; i < size; i++) {
        // Lê o valor atual do ADC. A tensão do microfone é convertida para
        // um número inteiro entre 0 e 4095 (resolução de 12 bits).
        buffer[i] = adc_read();
        
// MENSAGEM 2: Mostra o progresso do loop.
        // Imprime uma mensagem a cada 1000 amostras para não poluir o terminal.
        if (i % 1000 == 0) {
            printf("DEBUG: Capturando amostra %u...\n", i);
        }

        // Pausa a execução pelo tempo exato entre as amostras para manter
        // a taxa de amostragem definida. Ex: 1.000.000us / 8000Hz = 125us.
        // Isso é CRUCIAL para que o áudio não seja reproduzido acelerado ou lento.
        sleep_us(1000000 / SAMPLE_RATE);
    }
    // MENSAGEM 3: Avisa que o loop terminou.
    printf("DEBUG: Captura finalizada. Saindo da função.\n");
}

/**
 * @brief Reproduz as amostras de áudio do 'buffer' no buzzer.
 *
 * @param buffer Ponteiro para o array que contém as amostras de áudio gravadas.
 * @param size O número total de amostras a serem reproduzidas.
 */
void play_audio(uint16_t *buffer, size_t size) {
    // Loop para reproduzir cada uma das amostras gravadas.
    for (size_t i = 0; i < size; i++) {
    
        // --- ALTERAÇÃO CRÍTICA E CORREÇÃO FINAL ---
        // A amostra original do ADC vai de 0 a 4095. Precisamos convertê-la
        // para a nova escala do nosso PWM, que vai de 0 a 250.
        // 1. (float)buffer[i] / 4095.0f: Normaliza o valor para uma faixa de 0.0 a 1.0.
        // 2. * 250.0f: Escala esse valor normalizado para a nossa faixa de 0 a 250.
        uint16_t level = (uint16_t)(((float)buffer[i] / 4095.0f) * 250.0f);
        
        // Define o nível do duty cycle do PWM para o valor calculado.
        // Esta é a função correta e mais simples, pois usa o pino GPIO diretamente,
        // sem precisar da variável 'slice' neste escopo.
        pwm_set_gpio_level(PWM_AUDIO_OUT_PIN, level);

        // Pausa entre cada amostra para reproduzir o áudio na velocidade correta,
        // espelhando a pausa da captura.
        sleep_us(1000000 / SAMPLE_RATE);
    }
    
    // Ao final do loop, desliga o PWM (define o nível como 0) para silenciar o buzzer.
    pwm_set_gpio_level(PWM_AUDIO_OUT_PIN, 0);
}