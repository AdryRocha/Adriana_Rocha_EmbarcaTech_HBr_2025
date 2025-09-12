#include "sys/time.h"
#include "lwip/sockets.h"  /* socket(), recv(), write(), AF_* */
#include "lwip/netdb.h"    /* gethostbyname, struct hostent */
#include "lwip/inet.h"     /* htons, inet_* */
#include <string.h>
#include <errno.h>

#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0x40
#endif
int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen) {
    return lwip_write(sock, buf, buflen);
}

int transport_getdata(int sock, unsigned char* buf, int count) {
    return lwip_read(sock, buf, count);
}

int transport_getdatanb(int sock, unsigned char* buf, int count) {
    return lwip_recv(sock, buf, count, MSG_DONTWAIT);
}

int transport_open(const char* addr, int port) {
    int sock = -1;
    struct sockaddr_in server;
    struct hostent *hp;

    sock = lwip_socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Erro criando socket\n");
        return -1;
    }

    hp = gethostbyname(addr);
    if (!hp) {
        printf("Erro no DNS\n");
        lwip_close(sock);
        return -1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    memcpy(&server.sin_addr, hp->h_addr, hp->h_length);

    if (lwip_connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Erro ao conectar\n");
        lwip_close(sock);
        return -1;
    }

    return sock;
}

int transport_close(int sock) {
    return lwip_close(sock);
}

