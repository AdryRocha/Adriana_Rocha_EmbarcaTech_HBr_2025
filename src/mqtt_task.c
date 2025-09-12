#include "MQTTClient.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include <stdio.h>

// Inclui nossa implementação de rede
#include "paho_network.h"
#include "paho_timer.h"

// ========================================================================
// CONFIGURAÇÕES DE REDE - MQTT
// ========================================================================

// Servidor MQTT (SUBSTITUIR PELO SEU BROKER)
#define MQTT_BROKER_HOST            "bbbf88f16bbe4aa6ba8a25a2adde1d65.s1.eu.hivemq.cloud"
#define MQTT_BROKER_PORT            1883
#define MQTT_BROKER_PORT_SSL        8883        // Porta SSL/TLS

// Configurações de autenticação MQTT
#define MQTT_USE_AUTH               1       // true para habilitar autenticação
#define MQTT_USERNAME               "IotmyDevices"
#define MQTT_PASSWORD               "AtLrfD8GA4@VEK#"

// Configurações de conexão MQTT
#define MQTT_CLIENT_ID_PREFIX       "tinyml_gate_"
#define MQTT_CLIENT_ID              "tinyml_gate_pico_001"
#define MQTT_KEEP_ALIVE_SEC         60          // Keep alive: 60 segundos
#define MQTT_CONNECT_TIMEOUT_MS     10000       // Timeout conexão: 10s
#define MQTT_QOS_LEVEL              0           // QoS 0: At most once delivery

// Tópicos MQTT
#define MQTT_TOPIC_ROOT             "tinyml/iot/myDevices"
#define MQTT_TOPIC_DATA             MQTT_TOPIC_ROOT "/data"
#define MQTT_TOPIC_ALERTS           MQTT_TOPIC_ROOT "/alerts"
#define MQTT_TOPIC_STATUS           MQTT_TOPIC_ROOT "/status"
#define MQTT_TOPIC_CONFIG           MQTT_TOPIC_ROOT "/config"
#define MQTT_TOPIC_COMMANDS         MQTT_TOPIC_ROOT "/commands"

// Para teste
#define MQTT_SUB_TOPIC              MQTT_TOPIC_COMMANDS
#define MQTT_PUB_TOPIC              MQTT_TOPIC_STATUS

// Configurações de buffer MQTT
#define MQTT_BUFFER_SIZE            1024        // Buffer para mensagens MQTT
#define MQTT_MAX_MESSAGE_SIZE       512         // Tamanho máximo de mensagem

// Buffer de rede e MQTT
static unsigned char sendbuf[512];
static unsigned char readbuf[512];

// Função callback para mensagens recebidas
void messageArrived(MessageData* data) {
    printf("Mensagem recebida em %.*s: %.*s\n",
           data->topicName->lenstring.len, data->topicName->lenstring.data,
           (int)data->message->payloadlen, (char*)data->message->payload);
}

// Task MQTT
void mqtt_task(void *pvParameters) {
    int rc = 0;
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    printf("MQTT Task iniciada\n");

    // Aguarda um pouco para garantir que o WiFi esteja conectado
    vTaskDelay(pdMS_TO_TICKS(5000));

    // Inicializa rede usando nossa implementação
    NetworkInit(&network);
    
    printf("Tentando conectar ao broker %s:%d\n", MQTT_BROKER_HOST, MQTT_BROKER_PORT);
    
    if ((rc = NetworkConnect(&network, MQTT_BROKER_HOST, MQTT_BROKER_PORT)) != 0) {
        printf("Falha ao conectar no broker %s:%d (erro: %d)\n", MQTT_BROKER_HOST, MQTT_BROKER_PORT, rc);
        vTaskDelete(NULL);
        return;
    }
    printf("Conectado ao broker %s:%d\n", MQTT_BROKER_HOST, MQTT_BROKER_PORT);

    // Inicializa cliente MQTT
    MQTTClientInit(&client, &network, MQTT_CONNECT_TIMEOUT_MS, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Configura dados de conexão
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.keepAliveInterval = MQTT_KEEP_ALIVE_SEC;
    connectData.cleansession = 1;

    // Configurar autenticação se habilitada
    #if MQTT_USE_AUTH
    connectData.username.cstring = MQTT_USERNAME;
    connectData.password.cstring = MQTT_PASSWORD;
    #endif

    printf("Conectando cliente MQTT...\n");
    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Falha ao conectar MQTT, código %d\n", rc);
        NetworkDisconnect(&network);
        vTaskDelete(NULL);
        return;
    }
    printf("Cliente MQTT conectado com sucesso\n");

    // Subscribe em um tópico
    printf("Subscrevendo em %s\n", MQTT_SUB_TOPIC);
    if ((rc = MQTTSubscribe(&client, MQTT_SUB_TOPIC, QOS0, messageArrived)) != 0) {
        printf("Erro ao subscrever, código %d\n", rc);
    } else {
        printf("Subscrito em %s com sucesso\n", MQTT_SUB_TOPIC);
    }

    // Loop principal
    int count = 0;
    while (1) {
        // Mantém a conexão viva
        if ((rc = MQTTYield(&client, 1000)) != 0) {
            printf("Erro no MQTTYield: %d\n", rc);
            break;
        }

        // Publica mensagem de teste a cada 5 segundos
        if (++count >= 5) {
            MQTTMessage message;
            char payload[128];
            snprintf(payload, sizeof(payload), 
                    "{\"device\":\"pico_w\",\"status\":\"online\",\"tick\":%lu,\"heap\":%d}", 
                    xTaskGetTickCount(), 
                    xPortGetFreeHeapSize());

            message.qos = QOS0;
            message.retained = 0;
            message.payload = payload;
            message.payloadlen = strlen(payload);

            printf("Publicando em %s: %s\n", MQTT_PUB_TOPIC, payload);
            if ((rc = MQTTPublish(&client, MQTT_PUB_TOPIC, &message)) != 0) {
                printf("Erro ao publicar, código %d\n", rc);
            } else {
                printf("Mensagem publicada com sucesso\n");
            }
            count = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Cleanup em caso de erro
    printf("MQTT Task finalizando devido a erro\n");
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    vTaskDelete(NULL);
}