/**
 * @file    util.h
 * @author
 * @brief   Funções comuns
 * @version 0.1
 * @date
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "../include/config.h"
#include "../include/util.h"

/**
 * @brief Formata uma mensagem (msg) e a envia para o display
 * A mensagem formatada consta de 2 campos:
 * - campo tempo, com 5 caracteres que mostra o instante em segundos que a mensagem foi gerada: xxx.x
 * - campo mensagem que é a mensagem propriamente dita
 * @param q_handle, pilha que será utilizada para o envio
 * @param msg, mensagem que será adicionada
 */
void util_gera_e_envia_msg(QueueHandle_t q_handle, char * msg){
    char dest[QUEUE_MSG_LENGHT] = {0};

    uint32_t t0 = time_us_32() / 100000;
    uint16_t td = t0 % 10;
    uint16_t ts = (t0 / 10) % 1000;

    sprintf(dest, "%3d.%d %s", ts, td, msg);

    xQueueSend(q_handle, dest, portMAX_DELAY);
}