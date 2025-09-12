/**
 * @file    button_a.h
 * @author  Adriana - Elias - Vagner
 * @brief
 * @version 0.1
 * @date    25/08/2025
 */
#ifndef BUTTON_A_H
#define BUTTON_A_H


/**
 * @brief Task do botão A
 * Inicializa GPIO do botão
 * Monitora transições de estado do,botão :
 * - quando pressionado:
 *   - envia mensagem que foi pressionado para o display via pilha;
 *   - aguarda o LED estar disponível(MUTEX) para acessá-lo e manda uma nova mensagem para o display via pilha;
 * - quando solto:
 *   - libera o acesso ao LED(MUTEX);
 *   - Manda uma mensagem para o display via pilha
 */
void button_a_task(void *pvParameters);


#endif // BUTTON_A_H
