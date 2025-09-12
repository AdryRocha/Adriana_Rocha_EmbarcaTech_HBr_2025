/**
 * @file    util.h
 * @author  Adriana - Elias - Vagner
 * @brief   Funções comuns
 * @version 0.1
 * @date    2025-08-25
 */
#ifndef UTIL_H
#define UTIL_H


#include "FreeRTOS.h"
#include "queue.h"

/**
 * @brief Formata uma mensagem (msg) e a envia para o display
 * A mensagem formatada consta de 2 campos:
 * - campo tempo, com 5 caracteres que mostra o instante em segundos que a mensagem foi gerada: xxx.x
 * - campo mensagem que é a mensagem propriamente dita
 * @param q_handle, pilha que será utilizada para o envio
 * @param msg, mensagem que será adicionada
 */
void util_gera_e_envia_msg(QueueHandle_t q_handle, char * msg);


#endif // UTIL_H