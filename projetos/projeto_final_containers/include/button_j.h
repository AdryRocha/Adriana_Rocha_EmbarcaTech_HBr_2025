/**
 * @file    button_j.h
 * @author  Adriana - Elias - Vagner
 * @brief   Task para o botão J: Joystick
 * @version 0.1
 * @date    2025-08-25
 */
#ifndef BUTTON_J_H
#define BUTTON_J_H


/**
 * @brief Inicializa GPIO do botão C Joystick
 * Monitora transições de estado do,botão :
 *   - quando pressionado gera um semáforo
 */
void button_j_task(void *pvParameters);


#endif // BUTTON_J_H