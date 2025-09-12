#ifndef PAHO_NETWORK_H
#define PAHO_NETWORK_H

#include "MQTTClient.h"  // dá o typedef de Network e as assinaturas de Timer

void NetworkInit(Network *n);
int  NetworkConnect(Network *n, char *host, int port);
void NetworkDisconnect(Network *n);
int  NetworkRead(Network *n, unsigned char *buf, int len, int timeout_ms);
int  NetworkWrite(Network *n, unsigned char *buf, int len, int timeout_ms);

#endif