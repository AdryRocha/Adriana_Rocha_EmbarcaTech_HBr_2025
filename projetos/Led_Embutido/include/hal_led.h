
 #ifndef HAL_LED_H
 #define HAL_LED_H

 #include "pico/stdlib.h"

 // Pico W devices use a GPIO on the WIFI chip for the LED,
 // so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
 #ifdef CYW43_WL_GPIO_LED_PIN
 #include "pico/cyw43_arch.h"
 #endif

 #include <stdbool.h>

 /**
  * @brief Inicializa a camada de abstração de hardware para LEDs
  */
void hal_led_init(void);
 
 /**
  * @brief Alterna o estado do LED (liga se estiver desligado, desliga se estiver ligado)
  */
 void hal_led_toggle(void);
 
 /**
  * @brief Define o estado do LED (ligado ou desligado)
  * @param estado true para ligar o LED, false para desligar
  */
 void hal_led_set(bool estado);
 
 /**
  * @brief Alterna o estado do LED (liga se estiver desligado, desliga se estiver ligado)
  */
 void hal_led_toggle(void);

/**
 * @brief Define o estado do LED (ligado ou desligado)
 * @param estado true para ligar o LED, false para desligar
 */
void hal_led_set(bool estado);
 
/**
 * @brief Verifica se o LED está ligado
 * @return true se o LED estiver ligado, false se estiver desligado
 */
bool hal_led_get(void);

 #endif
 // Fim do arquivo hal_led.h