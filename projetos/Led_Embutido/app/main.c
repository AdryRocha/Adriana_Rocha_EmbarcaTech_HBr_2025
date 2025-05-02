 #include "pico/stdlib.h" // Biblioteca padrão do Raspberry Pi Pico

 #ifdef CYW43_WL_GPIO_LED_PIN // Verifica se o LED embutido está definido
 #include "pico/cyw43_arch.h" // Biblioteca para controle do Wi-Fi
 #endif

 #include "../include/hal_led.h"         // Inclui o cabeçalho da HAL de LED, onde as funções públicas são declaradas.

 int main() {
     // Inicializa a comunicação serial padrão
     stdio_init_all();
     if(cyw43_arch_init()) {
         return -1; // Falha na inicialização do módulo CYW43
     }
     
     // Inicializa a camada HAL para controle do LED
     hal_led_init();
     
     
     // Loop principal da aplicação
     while (true) {       
         hal_led_toggle();
         sleep_ms(500);
         }
 }