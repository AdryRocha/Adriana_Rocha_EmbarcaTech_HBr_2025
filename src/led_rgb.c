/**
 * @file    led_rgb.c
 * @author  Adriana - Elias - Vagner
 * @brief   Configura e inicializa os GPIOs dos LEDs, bem como seta os LEDs para uma determinada cor
 * @version 0.1
 * @date    2025-08-25
 */
#include "pico/stdlib.h"
#include "../include/config.h"
#include "../include/led_rgb.h"

static void init_led(uint gpio){
    gpio_set_dir(     gpio, GPIO_OUT);
    gpio_put(         gpio, 0);
    gpio_set_function(gpio, GPIO_FUNC_SIO);
}

/**
 * @brief Configura e inicializa os GPIOs dos LEDs
 * 
 */
void led_rgb_init(){
    init_led(LED_R_GPIO);
    init_led(LED_G_GPIO);
    init_led(LED_B_GPIO);

    led_rgb_set(LED_COR_RED);
    sleep_ms(250);
    led_rgb_set(LED_COR_GREEN);
    sleep_ms(250);
    led_rgb_set(LED_COR_BLUE);
    sleep_ms(250);
    led_rgb_set(LED_COR_BLACK);
}

/**
 * @brief Seta os LEDs para uma determinada cor
 * 
 * @param led_cor 
 */
void led_rgb_set(LedCor led_cor){
    gpio_put(LED_R_GPIO, led_cor == LED_COR_RED);
    gpio_put(LED_G_GPIO, led_cor == LED_COR_GREEN);
    gpio_put(LED_B_GPIO, led_cor == LED_COR_BLUE);
}