#include "utils.h"

// --- PINOS DE UTILIDADES PARA A BITDOGLAB v6 ---
#define RED_LED_PIN   15 // Perna Vermelha do LED RGB no GP15
#define GREEN_LED_PIN 16 // Perna Verde do LED RGB no GP16

#define BUTTON_A_PIN 5 // Botão A no GP5
#define BUTTON_B_PIN 6 // Botão B no GP6

void init_leds() {
    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT); // Define como saída.
    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);
}

void set_led_color(int color) {
    // Liga o LED correspondente à cor e garante que o outro está apagado.
    gpio_put(RED_LED_PIN, color == RED);
    gpio_put(GREEN_LED_PIN, color == GREEN);
}

void init_buttons() {
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);   // Define como entrada.
    gpio_pull_up(BUTTON_A_PIN); // Ativa o resistor de pull-up interno.

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
}

bool button_pressed(uint gpio) {
    // O pull-up interno inverte a lógica: o pino fica em nível baixo (0)
    // quando o botão é pressionado. Por isso usamos a negação `!`.
    return !gpio_get(gpio);
}