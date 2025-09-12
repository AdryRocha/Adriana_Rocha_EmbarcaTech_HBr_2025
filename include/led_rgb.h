/**
 * @file    led_rgb.h
 * @author  Ricardo e Vagner
 * @brief   Configura e inicializa os GPIOs dos LEDs, bem como seta os LEDs para uma determinada cor
 * @version 0.1
 * @date    2025-06-25
 */
#ifndef LED_RGB_H
#define LED_RGB_H


typedef enum {
    LED_COR_BLACK,
    LED_COR_RED,
    LED_COR_GREEN,
    LED_COR_BLUE,
} LedCor;

/**
 * @brief Configura e inicializa os GPIOs dos LEDs
 * 
 */
void led_rgb_init();

/**
 * @brief Seta os LEDs para uma determinada cor
 * 
 * @param led_cor 
 */
void led_rgb_set(LedCor led_cor);


#endif // LED_RGB_H