#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stddef.h>

// Inicializa a comunicação I2C e o display OLED.
void init_display();

// Limpa todo o conteúdo do display.
void clear_display();

// Desenha a forma de onda do áudio no display.
void show_waveform(uint16_t *buffer, size_t size);

#endif // DISPLAY_H