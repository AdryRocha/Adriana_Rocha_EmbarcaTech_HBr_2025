#include "../include/led_embutido.h"
#include "pico/stdlib.h"          // Biblioteca padrão do Raspberry Pi Pico

int led_embutido_init(void) {
// Inicialização do LED embutido é feita automaticamente por cyw43_arch_init() no SDK
// Inicializa o sistema Wi-Fi e o LED embutido
if (cyw43_arch_init() != 0) {
    return -1;  // Falha na inicialização
}
    return 0;  // Inicialização bem-sucedida
}
 
void led_embutido_set(bool estado) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, estado);  // Define o estado do LED embutido: ligado (true) ou desligado (false)  
}   


