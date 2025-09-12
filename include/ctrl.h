/**
 * @file    ctrl.h
 * @author  Adriana - Elias - Vagner
 * @brief   Cria e disponibiliza variáveis de semáforo e pilha
 * @version 0.1
 * @date    2025-08-25
 */
#ifndef CTRL_H
#define CTRL_H


#include "FreeRTOS.h"
#include "semphr.h"

extern QueueHandle_t     queue_handle;
extern SemaphoreHandle_t mutex_handler_led;
extern SemaphoreHandle_t semaphored_handler_joystick;

/**
 * @brief Inicializa as variáveis de semáforo e pilha
 * 
 */
void ctrl_init();


#endif // CTRL_H