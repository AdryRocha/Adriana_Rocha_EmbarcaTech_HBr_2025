#ifndef PAHO_TIMER_H
#define PAHO_TIMER_H

// Header compatível que não conflita com MQTTClient.h
// Use este header apenas se você precisar das definições em outros arquivos
// que NÃO incluem MQTTClient.h

// Inclui MQTTClient.h para ter acesso às definições corretas
#include "MQTTClient.h"

// Funções de timer disponíveis (implementadas em paho_timer.c)
// Estas funções já estão declaradas em MQTTClient.h como extern, 
// mas incluímos aqui para conveniência
#ifndef MQTT_CLIENT_H_TIMER_FUNCTIONS
void TimerInit(Timer* timer);
char TimerIsExpired(Timer* timer);
void TimerCountdownMS(Timer* timer, unsigned int ms);
void TimerCountdown(Timer* timer, unsigned int seconds);
int TimerLeftMS(Timer* timer);
#endif

// IMPORTANTE: 
// - Se você já incluiu MQTTClient.h, NÃO precisa incluir este header
// - A estrutura Timer já está definida em MQTTClient.h
// - As funções já estão declaradas como extern em MQTTClient.h
// - Este header é apenas uma conveniência para acessar as funções

#endif // PAHO_TIMER_H