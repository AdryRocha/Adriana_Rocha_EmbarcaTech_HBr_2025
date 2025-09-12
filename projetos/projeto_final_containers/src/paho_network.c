// paho_network.c - Implementação das funções de rede para MQTT
// Compatível com as definições em MQTTClient.h

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

// Headers do lwIP
#include "lwip/opt.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include <string.h>

// Headers do FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

// Incluir apenas o MQTTClient.h que já tem as definições corretas
#include "MQTTClient.h"

// Implementações das funções que são chamadas pelos ponteiros na estrutura Network
static int mqtt_read(Network* n, unsigned char* buffer, int len, int timeout_ms) {
    if (timeout_ms > 0) {
        struct timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;
        lwip_setsockopt(n->my_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    }
    return lwip_recv(n->my_socket, buffer, len, 0);
}

static int mqtt_write(Network* n, unsigned char* buffer, int len, int timeout_ms) {
    if (timeout_ms > 0) {
        struct timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;
        lwip_setsockopt(n->my_socket, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    }
    return lwip_send(n->my_socket, buffer, len, 0);
}

static void mqtt_disconnect(Network* n) {
    if (n->my_socket >= 0) {
        lwip_close(n->my_socket);
        n->my_socket = -1;
    }
}

// Função para inicializar a estrutura Network
void NetworkInit(Network* n) {
    n->my_socket = -1;
    n->mqttread = mqtt_read;
    n->mqttwrite = mqtt_write;
    n->disconnect = mqtt_disconnect;
}

// Função para conectar à rede
int NetworkConnect(Network* n, const char* host, int port) {
    struct sockaddr_in server_addr;
    struct hostent* server;

    // Criar socket
    n->my_socket = lwip_socket(AF_INET, SOCK_STREAM, 0);
    if (n->my_socket < 0) {
        printf("Erro ao criar socket\n");
        return -1;
    }

    // Resolver hostname
    printf("Resolvendo hostname: %s\n", host);
    server = lwip_gethostbyname(host);
    if (!server) {
        printf("Erro ao resolver hostname: %s\n", host);
        lwip_close(n->my_socket);
        n->my_socket = -1;
        return -2;
    }

    // Configurar endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(port);

    printf("Conectando ao servidor...\n");
    if (lwip_connect(n->my_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Erro ao conectar ao servidor\n");
        lwip_close(n->my_socket);
        n->my_socket = -1;
        return -3;
    }

    // Configurações opcionais do socket
    int yes = 1;
    lwip_setsockopt(n->my_socket, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));
    lwip_setsockopt(n->my_socket, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
    
    // Configurações de keepalive (se suportadas)
    int idle = 30, intvl = 10, cnt = 3;
    lwip_setsockopt(n->my_socket, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
    lwip_setsockopt(n->my_socket, IPPROTO_TCP, TCP_KEEPINTVL, &intvl, sizeof(intvl));
    lwip_setsockopt(n->my_socket, IPPROTO_TCP, TCP_KEEPCNT, &cnt, sizeof(cnt));

    printf("Conectado com sucesso!\n");
    return 0;
}

// Função para desconectar da rede
void NetworkDisconnect(Network* n) {
    if (n->disconnect) {
        n->disconnect(n);
}
}