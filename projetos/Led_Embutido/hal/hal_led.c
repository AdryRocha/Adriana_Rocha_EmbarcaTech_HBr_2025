#include "../include/hal_led.h"          // Inclui o cabeçalho da HAL de LED, onde as funções públicas são declaradas.
#include "../include/led_embutido.h"     // Inclui o controle de LED específico do hardware (como o LED embutido do Pico).

static bool led_state = false;           // Variável estática que armazena o estado atual do LED (ligado ou desligado).

/**
 * @brief Inicializa a camada de abstração de hardware para LEDs
 */

void hal_led_init(void) {
    led_embutido_init();                 // Inicializa o LED embutido (função específica do hardware).
    led_state = false;                   // Define o estado inicial do LED como desligado (false).
    led_embutido_set(led_state);         // Atualiza o LED físico para refletir esse estado (apaga o LED).
}
/**
 * @brief Alterna o estado do LED (liga se estiver desligado, desliga se estiver ligado)
 */
void hal_led_toggle(void) {
    led_state = !led_state;              // Inverte o estado atual: se estava ligado, desliga; se estava desligado, liga.
    led_embutido_set(led_state);         // Atualiza o LED físico com o novo estado.
}
/**
 * @brief Define o estado do LED (ligado ou desligado)
 * @param estado true para ligar o LED, false para desligar
 */
void hal_led_set(bool estado) {
    led_state = estado;                  // Atualiza a variável de estado interno com o valor passado.
    led_embutido_set(led_state);         // Atualiza o LED físico com o estado desejado (true = ligado, false = desligado).
}
/**
 * @brief Verifica se o LED está ligado
 * @return true se o LED estiver ligado, false se estiver desligado
 */
bool hal_led_get(void) {
    return led_state;           // Retorna o estado atual do LED (ligado ou desligado).
}
