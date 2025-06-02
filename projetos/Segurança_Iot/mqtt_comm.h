#ifndef MQTT_COMM_H
#define MQTT_COMM_H

#include "lwip/apps/mqtt.h"
#include <stdint.h>
#include <stddef.h>
#include "mqtt_comm.h"

#define MQTT_TOPIC_PUBLISH "escola/sala1/temperatura"
#define MQTT_TOPIC_SUBSCRIBE "escola/sala1/temperatura" // Pode ser o mesmo ou diferente
#define MQTT_DEFAULT_PORT 1883

// Estrutura para passar dados para os callbacks MQTT
typedef struct MQTT_CLIENT_DATA_T {
    uint8_t xor_key;
    uint32_t last_timestamp_received;
    // Adicione outros dados de estado do subscriber aqui, se necessário
} mqtt_client_data_t;


/**
 * @brief Configura e inicia a conexão com o broker MQTT.
 *
 * @param client_id ID do cliente MQTT.
 * @param broker_ip Endereço IP do broker MQTT.
 * @param user Usuário para autenticação (pode ser NULL).
 * @param pass Senha para autenticação (pode ser NULL).
 * @param app_data Ponteiro para dados da aplicação a serem passados aos callbacks.
 */
void mqtt_app_init(const char *client_id, const char *broker_ip, const char *user, const char *pass, mqtt_client_data_t* app_data);

/**
 * @brief Publica uma mensagem em um tópico MQTT.
 *
 * @param topic Tópico para publicação.
 * @param data Dados a serem publicados.
 * @param len Comprimento dos dados.
 * @param qos Qualidade de Serviço (0, 1 ou 2).
 * @param retain Flag de retenção (0 ou 1).
 */
void mqtt_app_publish(const char *topic, const uint8_t *data, size_t len, uint8_t qos, uint8_t retain);

/**
 * @brief Retorna o ponteiro do cliente MQTT.
 * Cuidado ao usar externamente, idealmente o cliente deve ser gerenciado internamente.
 * @return mqtt_client_t*
 */
mqtt_client_t* get_mqtt_client_instance(void);


#endif // MQTT_COMM_H