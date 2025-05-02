 // led_embutido.h - Interface pública do driver do LED embutido

 #ifndef LED_EMBUTIDO_H     // Proteção contra inclusões múltiplas
 #define LED_EMBUTIDO_H

 #include "pico/stdlib.h"
 #include <stdbool.h>  // Biblioteca padrão do Raspberry Pi Pico
 #include "pico/cyw43_arch.h" // Biblioteca para controle do Wi-Fi
 

 /**
 * @brief Inicializa o hardware do LED embutido
 * @return 0 se bem-sucedido, -1 caso contrário
 */
int led_embutido_init(void); // Inicializa o driver do LED embutido

 // Define o estado do LED embutido: ligado (true) ou desligado (false)
void led_embutido_set(bool estado); 


 #endif
 // Fim do arquivo led_embutido.h