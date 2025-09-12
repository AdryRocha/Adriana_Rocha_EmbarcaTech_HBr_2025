/**
 * @file    button_b.h
 * @author  Adriana - Elias - Vagner
 * @brief   Task para o botão B
 * @version 0.1
 * @date    2025-08-25
 */
#ifndef BUTTON_B_H
#define BUTTON_B_H


/**
 * @brief Task do botão B
 * Inicializa GPIO do botão
 * Monitora transições de estado do,botão :
 * - quando pressionado:
 *   - envia mensagem que foi pressionado para o display via pilha;
 *   - aguarda o LED estar disponível(MUTEX) para acessá-lo e manda uma nova mensagem para o display via pilha;
 * - quando solto:
 *   - libera o acesso ao LED(MUTEX);
 *   - Manda uma mensagem para o display via pilha
 */
void button_b_task(void *pvParameters);


#endif // BUTTON_B_H
