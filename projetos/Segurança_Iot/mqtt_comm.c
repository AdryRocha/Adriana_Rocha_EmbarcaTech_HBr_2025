#include "mqtt_comm.h"
#include "lwip/dns.h"
#include "pico/cyw43_arch.h" // Para cyw43_arch_lwip_begin/end
#include <stdio.h>
#include <string.h>
#include <time.h> // Para o timestamp na mensagem de log

// Declaração da função XOR que estará no main.c
extern void xor_encrypt(const uint8_t *input, uint8_t *output, size_t len, uint8_t key);

static mqtt_client_t *mqtt_client_inst = NULL;
static ip_addr_t mqtt_broker_addr;
static mqtt_client_data_t* client_app_data = NULL; // Para armazenar dados da aplicação para callbacks

#define MQTT_MAX_INCOMING_MSG_SIZE 256
static uint8_t incoming_msg_buffer[MQTT_MAX_INCOMING_MSG_SIZE];
static uint32_t incoming_msg_len = 0;

// --- Callbacks Internos do MQTT ---

static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
    mqtt_client_data_t* cb_data = (mqtt_client_data_t*)arg;
    printf("[%lu] MQTT: Publicação recebida no tópico '%s', tamanho total = %lu\n", (unsigned long)time(NULL), topic, tot_len);

    if (tot_len > MQTT_MAX_INCOMING_MSG_SIZE) {
        printf("[%lu] Erro: Mensagem MQTT muito grande (%lu bytes)!\n", (unsigned long)time(NULL), tot_len);
        incoming_msg_len = 0; // Descarta
        return;
    }
    incoming_msg_len = 0; // Prepara para nova mensagem
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    mqtt_client_data_t* cb_data = (mqtt_client_data_t*)arg;

    if ((incoming_msg_len + len) > MQTT_MAX_INCOMING_MSG_SIZE) {
        printf("[%lu] Erro: Buffer de mensagem MQTT estourou ao receber fragmento!\n", (unsigned long)time(NULL));
        incoming_msg_len = 0;
        return;
    }
    memcpy(incoming_msg_buffer + incoming_msg_len, data, len);
    incoming_msg_len += len;

    if (flags & MQTT_DATA_FLAG_LAST) {
        incoming_msg_buffer[incoming_msg_len] = '\0'; // Adiciona terminador nulo
        printf("[%lu] MQTT: Mensagem completa recebida (%lu bytes): Raw='%s'\n", (unsigned long)time(NULL), incoming_msg_len, (char*)incoming_msg_buffer);

        // Tentar decriptografar com XOR (Etapa 5)
        uint8_t decrypted_buffer[MQTT_MAX_INCOMING_MSG_SIZE];
        xor_encrypt(incoming_msg_buffer, decrypted_buffer, incoming_msg_len, cb_data->xor_key);
        decrypted_buffer[incoming_msg_len] = '\0'; // Adiciona terminador
        printf("[%lu] MQTT: Tentativa de Decriptografia (XOR com 0x%02X): '%s'\n", (unsigned long)time(NULL), cb_data->xor_key, (char*)decrypted_buffer);

        // Lógica da Etapa 6: Verificação de Timestamp (espera JSON em plain text ou JSON decriptografado)
        // O PDF indica que o JSON da Etapa 6 é enviado em plain text.
        // Se a Etapa 5 envia texto criptografado, e a Etapa 6 JSON não criptografado,
        // precisamos distinguir. Para este exemplo, vamos assumir que se o parse JSON falhar
        // no buffer original, tentamos no decriptografado. O ideal seria tópicos
        // diferentes ou um campo 'type' na mensagem.

        float valor_json = 0.0f;
        unsigned long ts_json = 0;
        int parsed_items = 0;

        // Tenta parsear o buffer original (caso a mensagem seja JSON não criptografado - Etapa 6)
        parsed_items = sscanf((char*)incoming_msg_buffer, "{\"valor\":%f,\"ts\":%lu}", &valor_json, &ts_json);

        if (parsed_items == 2) {
            printf("[%lu] MQTT: Mensagem JSON (original) Parseada: valor=%.1f, ts=%lu\n", (unsigned long)time(NULL), valor_json, ts_json);
        } else {
            // Tenta parsear o buffer decriptografado (se a Etapa 6 também criptografasse o JSON)
            // Ou se a Etapa 5 por acaso gerasse algo parecido com JSON após decriptografia (improvável)
            parsed_items = sscanf((char*)decrypted_buffer, "{\"valor\":%f,\"ts\":%lu}", &valor_json, &ts_json);
            if (parsed_items == 2) {
                printf("[%lu] MQTT: Mensagem JSON (decriptografada) Parseada: valor=%.1f, ts=%lu\n", (unsigned long)time(NULL), valor_json, ts_json);
            }
        }

        if (parsed_items == 2) { // Se conseguiu parsear JSON de alguma forma
            if (ts_json > cb_data->last_timestamp_received) {
                cb_data->last_timestamp_received = ts_json;
                printf("[%lu] MQTT: Nova leitura aceita (Timestamp): %.1f (ts: %lu)\n", (unsigned long)time(NULL), valor_json, ts_json);
                // Processar dados aqui
            } else {
                printf("[%lu] MQTT: Replay detectado ou timestamp antigo (Timestamp)! (ts recebido: %lu <= ultimo_ts: %lu)\n",
                       (unsigned long)time(NULL), ts_json, cb_data->last_timestamp_received);
            }
        } else {
            // Se não for JSON, pode ser a mensagem puramente textual criptografada da Etapa 5
            if (strcmp((char*)decrypted_buffer, "OlaMundoSeguro") == 0) {
                 printf("[%lu] MQTT: Mensagem da Etapa 5 ('OlaMundoSeguro') decriptografada com sucesso.\n", (unsigned long)time(NULL));
            } else {
                //printf("[%lu] MQTT: Mensagem recebida não é o JSON esperado nem 'OlaMundoSeguro' após XOR.\n", (unsigned long)time(NULL));
            }
        }
        incoming_msg_len = 0; // Reseta para a próxima mensagem
    }
}

static void mqtt_sub_request_cb(void *arg, err_t err) {
    if (err == ERR_OK) {
        printf("[%lu] MQTT: Subscrição ao tópico '%s' bem-sucedida!\n", (unsigned long)time(NULL), MQTT_TOPIC_SUBSCRIBE);
    } else {
        printf("[%lu] MQTT: Falha ao subscrever, erro: %d\n", (unsigned long)time(NULL), err);
    }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    mqtt_client_data_t* cb_data = (mqtt_client_data_t*)arg;
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("[%lu] MQTT: Conectado ao broker com sucesso!\n", (unsigned long)time(NULL));
        // Configura callbacks para mensagens recebidas
        mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, cb_data);

        // Subscreve ao tópico
        err_t err = mqtt_subscribe(client, MQTT_TOPIC_SUBSCRIBE, 0, mqtt_sub_request_cb, cb_data); // QoS 0
        if (err != ERR_OK) {
            printf("[%lu] MQTT: Erro ao iniciar subscrição: %d\n", (unsigned long)time(NULL), err);
        }
    } else {
        printf("[%lu] MQTT: Falha ao conectar ao broker, código: %d\n", (unsigned long)time(NULL), status);
        // Tentar reconectar ou tratar o erro
    }
}

static void mqtt_pub_request_cb(void *arg, err_t err) {
    if (err == ERR_OK) {
        //printf("[%lu] MQTT: Publicação enviada com sucesso!\n", (unsigned long)time(NULL)); // Log muito verboso
    } else {
        printf("[%lu] MQTT: Erro ao publicar: %d\n", (unsigned long)time(NULL), err);
    }
}

// --- Funções Públicas ---

void mqtt_app_init(const char *client_id, const char *broker_ip, const char *user, const char *pass, mqtt_client_data_t* app_data) {
    printf("[%lu] MQTT: Inicializando cliente...\n", (unsigned long)time(NULL));
    client_app_data = app_data; // Armazena os dados da aplicação

    // Resolução DNS do broker_ip se não for um IP literal
    // Para simplificar, vamos assumir que broker_ip é um IP literal por enquanto.
    // Se fosse um hostname, precisaria de dns_gethostbyname.
    if (ipaddr_aton(broker_ip, &mqtt_broker_addr) == 0) {
        printf("[%lu] MQTT: Erro ao converter IP do broker: %s\n", (unsigned long)time(NULL), broker_ip);
        // Tentar resolução DNS (exemplo básico, pode precisar de espera e callback)
        // dns_init(); // Certifique-se que o DNS está inicializado
        // err_t dns_err = dns_gethostbyname(broker_ip, &mqtt_broker_addr, dns_found_callback, NULL);
        // if (dns_err == ERR_INPROGRESS) { /* Esperar callback */ } else if (dns_err != ERR_OK) { /* Erro */ }
        return; // Falha na conversão/resolução imediata
    }
     printf("[%lu] MQTT: IP do Broker: %s\n", (unsigned long)time(NULL), ipaddr_ntoa(&mqtt_broker_addr));


    mqtt_client_inst = mqtt_client_new();
    if (mqtt_client_inst == NULL) {
        printf("[%lu] MQTT: Falha ao criar cliente MQTT.\n", (unsigned long)time(NULL));
        return;
    }

    struct mqtt_connect_client_info_t ci;
    memset(&ci, 0, sizeof(ci)); // Limpa a estrutura
    ci.client_id = client_id;
    ci.client_user = user;
    ci.client_pass = pass;
    ci.keep_alive = 60; // Keep alive em segundos
    // ci.will_topic = ... (opcional)
    // ci.will_msg = ... (opcional)
    // ci.will_qos = ... (opcional)
    // ci.will_retain = ... (opcional)

    // É importante proteger o acesso LwIP com cyw43_arch_lwip_begin/end
    // se estiver chamando funções LwIP de uma thread diferente da LwIP
    // ou se houver preemptividade. Com threadsafe_background, isso é menos crítico
    // para chamadas que enfileiram trabalho para a thread LwIP, mas é uma boa prática.
    cyw43_arch_lwip_begin();
    err_t err = mqtt_client_connect(mqtt_client_inst, &mqtt_broker_addr, MQTT_DEFAULT_PORT,
                                    mqtt_connection_cb, client_app_data, &ci);
    cyw43_arch_lwip_end();

    if (err != ERR_OK) {
        printf("[%lu] MQTT: Erro ao iniciar conexão: %d\n", (unsigned long)time(NULL), err);
    } else {
        printf("[%lu] MQTT: Conexão com broker iniciada...\n", (unsigned long)time(NULL));
    }
}

void mqtt_app_publish(const char *topic, const uint8_t *data, size_t len, uint8_t qos, uint8_t retain) {
    if (mqtt_client_inst == NULL || !mqtt_client_is_connected(mqtt_client_inst)) {
        printf("[%lu] MQTT: Cliente não conectado. Não é possível publicar.\n", (unsigned long)time(NULL));
        return;
    }

    cyw43_arch_lwip_begin();
    err_t err = mqtt_publish(mqtt_client_inst, topic, data, len, qos, retain, mqtt_pub_request_cb, NULL);
    cyw43_arch_lwip_end();

    if (err != ERR_OK) {
        printf("[%lu] MQTT: Erro ao publicar mensagem: %d\n", (unsigned long)time(NULL), err);
    } else {
         //printf("[%lu] MQTT: Mensagem para '%s' enfileirada para publicação.\n", (unsigned long)time(NULL), topic); // Verboso
    }
}

mqtt_client_t* get_mqtt_client_instance(void) {
    return mqtt_client_inst;
}