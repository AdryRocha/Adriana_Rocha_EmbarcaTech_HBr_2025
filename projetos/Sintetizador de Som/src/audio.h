#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <stddef.h>

// Inicializa o ADC para ler do microfone.
void init_adc();

// Inicializa o PWM para enviar som ao buzzer.
void init_pwm();

// Captura um número 'size' de amostras e as armazena no 'buffer'.
void capture_audio(uint16_t *buffer, size_t size);

// Reproduz as amostras de áudio do 'buffer'.
void play_audio(uint16_t *buffer, size_t size);

#endif // AUDIO_H