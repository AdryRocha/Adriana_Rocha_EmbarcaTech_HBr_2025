#include <stdio.h>
#include <string.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h" // Para cyw43_arch_lwip_begin/end e status
#include "hardware/rtc.h"    // Para RTC
#include "wifi_conn.h"
#include "mqtt_comm.h"

// --- Definições de Configuração ---
#define WIFI_SSID "ALEX_2G"         // Substitua pelo seu SSID
#define WIFI_PASSWORD "12345678" // Substitua pela sua senha
#define MQTT_BROKER_IP "192.168.0.171" // Substitua pelo IP do seu PC
#define MQTT_CLIENT_ID "BitDogLabPicoW"
#define MQTT_USER "aluno"             // Usuário MQTT (definido no mosquitto_passwd)
#define MQTT_PASSWORD "test123"       // Senha MQTT (definida no mosquitto_passwd)

#define XOR_KEY 0xAB

// --- Variáveis Globais de Estado ---
static int etapa_atual = 3; // Começa na etapa 3 após conexão Wi-Fi e MQTT
static int contador_etapa = 0;
static mqtt_client_data_t app_mqtt_data; // Dados para os callbacks MQTT

// --- Funções Utilitárias ---

// Função XOR para criptografia leve (Etapa 5)
// Esta função é chamada por mqtt_comm.c, por isso a declaração extern lá.
void xor_encrypt(const uint8_t *input, uint8_t *output, size_t len, uint8_t key) {
    for (size_t i = 0; i < len; ++i) {
        output[i] = input[i] ^ key;
    }
}

// Função para inicializar o RTC (opcional, para timestamps mais precisos se sincronizado)
void init_real_time_clock(void) {
    // Exemplo: definir uma data e hora iniciais.
    // Idealmente, sincronizar via NTP se houver conectividade com a internet e servidor NTP.
    // datetime_t t = {
    //     .year  = 2024,
    //     .month = 5,
    //     .day   = 23,
    //     .dotw  = 5, // 0=Dom, 1=Seg, ...
    //     .hour  = 12,
    //     .min   = 00,
    //     .sec   = 00
    // };
    // rtc_init();
    // rtc_set_datetime(&t);
    // Aguarde um pouco para o RTC estabilizar (se necessário)
    // sleep_us(64);
    printf("[%lu] RTC: Inicializado (sem sincronização NTP neste exemplo).\n", (unsigned long)time(NULL));
}


// --- Função Principal ---
int main() {
    stdio_init_all();
    sleep_ms(3000); // Aguarda para abrir o monitor serial

    printf("=== Iniciando Sistema de Segurança IoT ===\n");

    // Inicializa dados da aplicação para MQTT
    app_mqtt_data.xor_key = XOR_KEY;
    app_mqtt_data.last_timestamp_received = 0;

    init_real_time_clock(); // Tenta inicializar o RTC

    // Inicializa e conecta ao Wi-Fi
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_BRAZIL)) {
         printf("Falha ao inicializar Wi-Fi com país.\n");
        return -1;
    }
    printf("Wi-Fi inicializado com país.\n");


    if (connect_to_wifi(WIFI_SSID, WIFI_PASSWORD) != 0) {
        printf("Não foi possível conectar ao Wi-Fi. Verifique as credenciais e a rede.\n");
        // Loop infinito ou tratamento de erro mais robusto
        while(true) { tight_loop_contents(); }
    }

    // Configura e conecta ao MQTT
    // (A conexão real e subscrição acontecem de forma assíncrona)
    mqtt_app_init(MQTT_CLIENT_ID, MQTT_BROKER_IP, MQTT_USER, MQTT_PASSWORD, &app_mqtt_data);

    unsigned long last_publish_time = 0;
    const unsigned long PUBLISH_INTERVAL_MS = 10000; // Publica a cada 10 segundos

    while (true) {
        // O processamento de rede (Wi-Fi e LwIP/MQTT) é feito em segundo plano
        // pela biblioteca pico_cyw43_arch_lwip_threadsafe_background.
        // Não é necessário cyw43_arch_poll() explícito no loop principal.

        unsigned long current_time_ms = to_ms_since_boot(get_absolute_time());

        if ( (current_time_ms - last_publish_time > PUBLISH_INTERVAL_MS) &&
             get_mqtt_client_instance() != NULL &&
             mqtt_client_is_connected(get_mqtt_client_instance()) ) {

            last_publish_time = current_time_ms;
            contador_etapa++;

            switch (etapa_atual) {
                case 3: {
                    printf("[%lu] === Etapa 3: Publicação simples ===\n", (unsigned long)time(NULL));
                    const char *msg_simples = "Teste MQTT PicoW - Etapa 3";
                    mqtt_app_publish(MQTT_TOPIC_PUBLISH, (const uint8_t*)msg_simples, strlen(msg_simples), 0, 0);
                    printf("[%lu] Publicado: %s\n", (unsigned long)time(NULL), msg_simples);

                    if (contador_etapa >= 2) { // Publica 2x antes de mudar
                        etapa_atual = 4; // Na verdade, a Etapa 4 é sobre a configuração do broker
                                         // e o Pico já está usando autenticação.
                                         // Vamos pular para a 5 para fins de demonstração de payload.
                        etapa_atual = 5;
                        contador_etapa = 0;
                        printf("[%lu] Mudando para Etapa 5.\n", (unsigned long)time(NULL));
                    }
                    break;
                }

                // A Etapa 4 (Autenticação) é implicitamente testada pela conexão.
                // Se o broker exigir e as credenciais estiverem erradas, não conectará.
                // O código da placa já envia credenciais.

                case 5: {
                    printf("[%lu] === Etapa 5: Publicação com criptografia XOR ===\n", (unsigned long)time(NULL));
                    const char *msg_original = "OlaMundoSeguro";
                    uint8_t msg_criptografada[64]; // Buffer para a mensagem criptografada
                    size_t len_original = strlen(msg_original);

                    xor_encrypt((const uint8_t*)msg_original, msg_criptografada, len_original, XOR_KEY);

                    printf("[%lu] Original: %s | Criptografada (Hex): ", (unsigned long)time(NULL), msg_original);
                    for(size_t i=0; i<len_original; ++i) printf("%02X ", msg_criptografada[i]);
                    printf("\n");

                    mqtt_app_publish(MQTT_TOPIC_PUBLISH, msg_criptografada, len_original, 0, 0);

                    if (contador_etapa >= 2) {
                        etapa_atual = 6;
                        contador_etapa = 0;
                        printf("[%lu] Mudando para Etapa 6.\n", (unsigned long)time(NULL));
                    }
                    break;
                }

                case 6: {
                    printf("[%lu] === Etapa 6: Publicação com timestamp ===\n", (unsigned long)time(NULL));
                    char buffer_json[128];
                    unsigned long timestamp_msg;

                    // Tenta usar o RTC se disponível e válido
                    datetime_t dt_now;
                    if (rtc_get_datetime(&dt_now) && rtc_running()) {
                         // Converte datetime_t para timestamp Unix (simplificado, não considera timezone)
                        // Esta é uma conversão muito básica e pode não ser precisa.
                        // Para um timestamp Unix real, seria necessário uma biblioteca de tempo mais completa.
                        struct tm current_tm = {
                            .tm_sec = dt_now.sec, .tm_min = dt_now.min, .tm_hour = dt_now.hour,
                            .tm_mday = dt_now.day, .tm_mon = dt_now.month - 1, .tm_year = dt_now.year - 1900
                        };
                        timestamp_msg = mktime(&current_tm);
                        if (timestamp_msg == (time_t)-1) { // mktime pode falhar
                             timestamp_msg = (unsigned long)time(NULL); // Fallback para time() do sistema
                             if (timestamp_msg == 0 || timestamp_msg == (unsigned long)-1) { // Outro fallback
                                timestamp_msg = (current_time_ms / 1000) + contador_etapa * 10; // Simulado
                             }
                        }
                    } else {
                        timestamp_msg = (current_time_ms / 1000) + contador_etapa * 10; // Simulado
                    }


                    float valor_simulado = 20.0f + (contador_etapa % 100) / 10.0f; // Valor entre 20.0 e 29.9
                    snprintf(buffer_json, sizeof(buffer_json), "{\"valor\":%.1f,\"ts\":%lu}", valor_simulado, timestamp_msg);
                    printf("[%lu] Enviando: %s\n", (unsigned long)time(NULL), buffer_json);

                    mqtt_app_publish(MQTT_TOPIC_PUBLISH, (const uint8_t*)buffer_json, strlen(buffer_json), 0, 0);

                    if (contador_etapa >= 2) {
                        etapa_atual = 3; // Reinicia o ciclo
                        contador_etapa = 0;
                        printf("[%lu] Reiniciando ciclo para Etapa 3.\n", (unsigned long)time(NULL));
                    }
                    break;
                }
            } // Fim do switch
        } // Fim do if de publicar

        sleep_ms(100); // Pequeno delay para não sobrecarregar o processador em um loop vazio.
                       // A thread de rede LwIP roda em background.
    } // Fim do while(true)

    return 0; // Nunca alcançado
}