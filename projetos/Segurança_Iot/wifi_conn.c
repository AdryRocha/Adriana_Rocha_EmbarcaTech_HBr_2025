#include <pico/cyw43_arch.h>
#include <stdio.h>
#include "wifi_conn.h"
//#include "lwip/apps/mqtt.h"
#include "lwipopts.h"

int connect_to_wifi(const char *ssid, const char *password) {
    printf("Conectando ao Wi-Fi: %s\n", ssid);

    // cyw43_arch_init() é chamado pelo pico_cyw43_arch_lwip_threadsafe_background
    // ou precisa ser chamado se não estiver usando o modo background.
    // No setup do CMakeLists.txt, ele já será inicializado.

    cyw43_arch_enable_sta_mode();
    printf("Modo STA habilitado.\n");

    if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha ao conectar ao Wi-Fi.\n");
        return -1;
    } else {
        printf("Conectado com sucesso ao Wi-Fi!\n");
        printf("IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_default)));
        return 0;
    }
}