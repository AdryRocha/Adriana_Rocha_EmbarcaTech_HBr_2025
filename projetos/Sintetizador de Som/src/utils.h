#ifndef UTILS_H
#define UTILS_H

#include "pico/stdlib.h"
#include <stdbool.h>

// Constantes para fácil controle das cores do LED.
#define NONE  0
#define RED   1
#define GREEN 2

// Inicializa os pinos do LED RGB.
void init_leds();

// Define a cor do LED (vermelho, verde ou nenhuma).
void set_led_color(int color);

// Inicializa os pinos dos botões A e B.
void init_buttons();

// Verifica se um botão específico foi pressionado.
bool button_pressed(uint gpio);

#endif // UTILS_H