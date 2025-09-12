// paho_timer.c - Implementação de timer para MQTT usando FreeRTOS
// Compatível com as definições em MQTTClient.h

#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

// Incluir apenas o MQTTClient.h que já tem as definições corretas
#include "MQTTClient.h"

// Implementações das funções de timer
void TimerInit(Timer* timer) {
    timer->end_time = 0;
}

char TimerIsExpired(Timer* timer) {
    uint32_t now = xTaskGetTickCount();
    return (now >= timer->end_time) ? 1 : 0;
}

void TimerCountdownMS(Timer* timer, unsigned int ms) {
    uint32_t now = xTaskGetTickCount();
    timer->end_time = now + pdMS_TO_TICKS(ms);
}

void TimerCountdown(Timer* timer, unsigned int seconds) {
    TimerCountdownMS(timer, seconds * 1000);
}

int TimerLeftMS(Timer* timer) {
    uint32_t now = xTaskGetTickCount();
    if (now >= timer->end_time) {
        return 0;
    }
    return pdTICKS_TO_MS(timer->end_time - now);
}