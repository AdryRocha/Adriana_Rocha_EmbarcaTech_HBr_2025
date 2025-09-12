/**
 * @file    display_gate.c
 * @author
 * @brief   Task que mostra mensagens da file e requisições do semáforo no OLED.
 * Simula um dispositivo lento
 * @version 0.1
 * @date
 */
#include <string.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include <stdio.h> // Para snprintf
#include "../include/config.h"
#include "../include/ctrl.h"
#include "../include/display_gate.h"
#include "../include/mpu6050.h"
#include "oled/display.h"
#include "oled/i2c.h"

static char display_buffer[8][QUEUE_MSG_LENGHT] = {0};

/**
 * @brief Atualiza o display
 *
 * @param msg
 */
static void add_msg(char * msg){
    // move as mensagens anteriores para baixo
    for(int i=7; i>0; i--){
        strncpy(display_buffer[i], display_buffer[i-1], QUEUE_MSG_LENGHT);
    }

    // adiciona a nova mensagem
    strncpy(display_buffer[0], msg, QUEUE_MSG_LENGHT - 1);
    display_buffer[0][QUEUE_MSG_LENGHT - 1] = '\0';

    // limpa o display
    display_fill(false, false);

    // preenche o display com todas as 8 linhas
    for(int i=0; i<8; i++){
        display_msg(false, 0, i, display_buffer[i]);
    }

    // atualiza o display uma única vez
    display_update();
}


void display_gate_task(void *pvParameters){
    char bufferMsg[QUEUE_MSG_LENGHT] = {0};
    mpu6050_data_t mpu_data;
    QueueHandle_t mpu_queue_handle = (QueueHandle_t)mpu6050_get_queue();

    // Variável para controlar qual eixo exibir
    int axis_to_display = 0; // 0=Ax, 1=Ay, 2=Az

    display_init();

    // Variáveis para controlar o blink do display
    const int BLINK_DURATION_MS = 5000;
    const int BLINK_INTERVAL_MS = 500;
    const int BLINK_COUNT = BLINK_DURATION_MS / BLINK_INTERVAL_MS;

    for (int i = 0; i < BLINK_COUNT; i++) {
        if (i % 2 == 0) {
            display_msg(true, 1, 1, "::  TinyML  ::");
            display_msg(true, 1, 3, "   Movement   ");
            display_msg(true, 1, 5, "Classification");
        } else {
            display_fill(false, false);
        }
        vTaskDelay(pdMS_TO_TICKS(BLINK_INTERVAL_MS));
    }

    display_fill(false, false);


    while(1){
        if(xSemaphoreTake(semaphored_handler_joystick, 0) == pdTRUE){
            // Mantém o sistema de log com rolagem para botões
            add_msg(MSG_BUTTON_SEMAPHORO);
        }else if (xQueueReceive(queue_handle, bufferMsg, portMAX_DELAY) == pdTRUE) {
            add_msg(bufferMsg);
        }else if (xQueueReceive(mpu_queue_handle, &mpu_data, 0) == pdTRUE) {
            char accel_msg[QUEUE_MSG_LENGHT];

            // Limpa a tela antes de exibir os novos dados do acelerômetro
            display_fill(false, false);

            //display_msg(true, 1, 1, "::  TinyML  ::");

            // Formata e exibe o eixo X na linha 1
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ax: %.1f", mpu_data.accel_x_ms2);
            display_msg(false, 0, 1, accel_msg);

            // Formata e exibe o eixo Y na linha 3
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ay: %.1f", mpu_data.accel_y_ms2);
            display_msg(false, 0, 3, accel_msg);

            // Formata e exibe o eixo Z na linha 5
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Az: %.1f", mpu_data.accel_z_ms2);
            display_msg(false, 0, 5, accel_msg);

            // Formata e exibe a Magnitude da Aceleração na linha 7
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "A RMS: %.1f", mpu_data.accel_magnitude_ms2);
            display_msg(false, 0, 7, accel_msg);

            // Atualiza o display com todos os novos dados de uma vez
            display_update();
        }

        // Atualiza o display a cada 100ms
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
