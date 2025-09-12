/**
 * @file    display_gate.c
 * @author  Adriana - Elias - Vagner
 * @brief   Task que mostra mensagens da file e requisições do semáforo no OLED.
 * Simula um dispositivo lento
 * @version 0.1
 * @date
 */
#include <stdio.h> // Para snprintf
 #include <string.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "../include/config.h"
#include "../include/ctrl.h"
#include "../include/display_gate.h"
#include "mpu6050.h"
#include "oled/display.h"
#include "oled/i2c.h"

static char display_buffer[8][QUEUE_MSG_LENGHT] = {0};

/**
 * @brief Atualiza o display, simulando dispositivo lento
 *
 * @param msg
 */

/**
static void add_msg(char * msg){
    // move as mensagens anterios para baixo
    for(int i=8;i>0;i--){
        for(int j=0;j<QUEUE_MSG_LENGHT;j++){
            display_buffer[i][j] = display_buffer[i-1][j];
        }
    }
    // adiciona a nova mensagem
    for(int i=0;i<QUEUE_MSG_LENGHT-1;i++){
        display_buffer[0][i] = msg[i];
    }
    // limpa o display
    display_fill(false, false);

    // preenche o display, menos a primeira linha
    for(int i=1;i<8;i++)
        display_msg(false, 0, i, display_buffer[i]);

    // Marca a onde está escrevendo e depois escreve o valor para simular dispositivo lento
    bool reached_the_end = false;
    for(int i=0;i<16;i++){
        display_invert_char(true, i, 0);
        vTaskDelay(pdMS_TO_TICKS(50));
        display_invert_char(false, i, 0);
        if(!reached_the_end){
            char msg_c[2] = {0};
            msg_c[0] = display_buffer[0][i];
            if(msg_c[0]){
                display_msg(false, i, 0, msg_c);
            }else{
                reached_the_end = true;
            }
        }
        display_update();
    }
    vTaskDelay(pdMS_TO_TICKS(50));
}
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


/**
 * @brief Task que mostra mensagens da file e requisições do semáforo no OLED.
 * Simula um dispositivo lento
 */
// void display_gate_task(){
//     char bufferMsg[QUEUE_MSG_LENGHT] = {0};
//     // Estrutura para armazenar os dados do MPU6500
//     mpu6500_data_t mpu_data;
//     // Obtém o handle da fila do MPU6500.
//     // É necessário um cast pois mpu6500_get_queue retorna void*.
//     QueueHandle_t mpu_queue_handle = (QueueHandle_t)mpu6500_get_queue();

//     display_init();
//     display_msg(true, 1, 1, "::  TinyML  ::");
//     display_msg(true, 1, 3, "   Movement   ");
//     display_msg(true, 1, 5, "Classification");
//     vTaskDelay(pdMS_TO_TICKS(1000));
//     display_fill(false, false);
//     // display_msg(false, 0, 1, "    PRESSIONE");
//     // display_msg(false, 0, 3, "   ALGUM BOTAO");
//     // display_msg(false, 0, 5, "       OU");
//     // display_msg(true,  0, 7, "    JOYSTICK");

//     while(1){
//         if(xSemaphoreTake(semaphored_handler_joystick, 0) == pdTRUE){
//             add_msg(MSG_BUTTON_SEMAPHORO);
//         }else if (xQueueReceive(queue_handle, bufferMsg, 0) == pdTRUE) {
//             add_msg(bufferMsg);
//         }else if (xQueueReceive(mpu_queue_handle, &mpu_data, 0) == pdTRUE) { // Tenta receber dados do MPU6500
//             char accel_msg[QUEUE_MSG_LENGHT];
//             // Formata os dados de aceleração em uma string
//             // snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ax:%.2f Ay:%.2f Az:%.2f",
//             //          mpu_data.accel_x_g, mpu_data.accel_y_g, mpu_data.accel_z_g);
//             // add_msg(accel_msg); // Exibe a mensagem no display
//             snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ax:%.4f",mpu_data.accel_x_g);
//             add_msg(accel_msg); // Exibe a mensagem no display
//             snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ay:%.4f",mpu_data.accel_y_g);
//             add_msg(accel_msg); // Exibe a mensagem no display
//             snprintf(accel_msg, QUEUE_MSG_LENGHT, "Az:%.4f",mpu_data.accel_z_g);
//             add_msg(accel_msg); // Exibe a mensagem no display
//         }else{
//             vTaskDelay(pdMS_TO_TICKS(50));
//         }
//     }
// }
//
/**
void display_gate_task(){
    char bufferMsg[QUEUE_MSG_LENGHT] = {0};
    // Estrutura para armazenar os dados do MPU6500
    mpu6500_data_t mpu_data;
    // Obtém o handle da fila do MPU6500.
    // É necessário um cast pois mpu6500_get_queue retorna void*.
    QueueHandle_t mpu_queue_handle = (QueueHandle_t)mpu6500_get_queue();

    display_init();
    display_msg(true, 1, 1, "::  TinyML  ::");
    display_msg(true, 1, 3, "   Movement   ");
    display_msg(true, 1, 5, "Classification");
    vTaskDelay(pdMS_TO_TICKS(5000));
    display_fill(false, false);

    while(1){
        if(xSemaphoreTake(semaphored_handler_joystick, 0) == pdTRUE){
            add_msg(MSG_BUTTON_SEMAPHORO);
        }else if (xQueueReceive(queue_handle, bufferMsg, 0) == pdTRUE) {
            add_msg(bufferMsg);
        }else if (xQueueReceive(mpu_queue_handle, &mpu_data, 0) == pdTRUE) { // Tenta receber dados do MPU6500
            char accel_msg[QUEUE_MSG_LENGHT];
            // Formata e exibe os dados de aceleração
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ax:%.4f",mpu_data.accel_x_g);
            add_msg(accel_msg);
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ay:%.4f",mpu_data.accel_y_g);
            add_msg(accel_msg);
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Az:%.4f",mpu_data.accel_z_g);
            add_msg(accel_msg);
        }

        // Adiciona uma pausa de 1s aqui para controlar a taxa de atualização
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
*/

/**
void display_gate_task(){
    char bufferMsg[QUEUE_MSG_LENGHT] = {0};
    mpu6500_data_t mpu_data;
    QueueHandle_t mpu_queue_handle = (QueueHandle_t)mpu6500_get_queue();

    display_init();

    // --- Início da nova lógica para piscar ---
    const int BLINK_DURATION_MS = 5000;
    const int BLINK_INTERVAL_MS = 500;
    const int BLINK_COUNT = BLINK_DURATION_MS / BLINK_INTERVAL_MS;

    for (int i = 0; i < BLINK_COUNT; i++) {
        if (i % 2 == 0) {
            // Estado ON: Mostra a mensagem
            display_msg(true, 1, 1, "::  TinyML  ::");
            display_msg(true, 1, 3, "   Movement   ");
            display_msg(true, 1, 5, "Classification");
        } else {
            // Estado OFF: Limpa a tela
            display_fill(false, false);
        }
        // Pausa para o próximo estado
        vTaskDelay(pdMS_TO_TICKS(BLINK_INTERVAL_MS));
    }

    // Garante que a tela esteja limpa antes de entrar no laço principal
    display_fill(false, false);
    // --- Fim da nova lógica ---

    while(1){
        if(xSemaphoreTake(semaphored_handler_joystick, 0) == pdTRUE){
            add_msg(MSG_BUTTON_SEMAPHORO);
        }else if (xQueueReceive(queue_handle, bufferMsg, 0) == pdTRUE) {
            add_msg(bufferMsg);
        }else if (xQueueReceive(mpu_queue_handle, &mpu_data, 0) == pdTRUE) {
            char accel_msg[QUEUE_MSG_LENGHT];
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ax:%.4f",mpu_data.accel_x_g);
            add_msg(accel_msg);
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ay:%.4f",mpu_data.accel_y_g);
            add_msg(accel_msg);
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Az:%.4f",mpu_data.accel_z_g);
            add_msg(accel_msg);
        }
        //  Adiciona uma pausa de 1s aqui para controlar a taxa de atualização
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
*/

void display_gate_task(void *pvParameters){
    // --- MENSAGEM DE DEBUG: INÍCIO DA TAREFA ---
    printf("DEBUG: display_gate_task iniciada.\n");

    char bufferMsg[QUEUE_MSG_LENGHT] = {0};
    mpu6050_data_t mpu_data;
    QueueHandle_t mpu_queue_handle = (QueueHandle_t)mpu6050_get_queue();

    // --- MENSAGEM DE DEBUG: ANTES DO DISPLAY_INIT ---
    printf("DEBUG: prestes a chamar display_init().\n");
    display_init();
    // --- MENSAGEM DE DEBUG: DEPOIS DO DISPLAY_INIT ---
    printf("DEBUG: display_init() concluido com sucesso.\n");

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

    /**
    while(1){
        if(xSemaphoreTake(semaphored_handler_joystick, 0) == pdTRUE){
            // Para mensagens de botão, usamos a fonte normal para não ocupar a tela toda
            add_msg(MSG_BUTTON_SEMAPHORO);
        } else if (xQueueReceive(queue_handle, bufferMsg, 0) == pdTRUE) {
            add_msg(bufferMsg);
        } else if (xQueueReceive(mpu_queue_handle, &mpu_data, 0) == pdTRUE) {
            char accel_msg[QUEUE_MSG_LENGHT];

            // Limpa a tela antes de desenhar o texto grande
            display_fill(false, false);

            // Alterna a exibição entre os eixos
            switch(axis_to_display) {
                case 0:
                    snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ax:%.2f", mpu_data.accel_x_g);
                    break;
                case 1:
                    snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ay:%.2f", mpu_data.accel_y_g);
                    break;
                case 2:
                    snprintf(accel_msg, QUEUE_MSG_LENGHT, "Az:%.2f", mpu_data.accel_z_g);
                    break;
            }

            // Exibe a mensagem com a fonte grande na segunda linha (para centralizar um pouco)
            display_msg_large(false, 0, 2, accel_msg);
            display_update();

            // Atualiza para o próximo eixo no próximo ciclo
            axis_to_display = (axis_to_display + 1) % 3;

        }

        // Atualiza o display a cada segundo
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
*/
    display_fill(false, false);
    printf("DEBUG: Entrando no loop while(1).\n");

    while(1){
        if(xSemaphoreTake(semaphored_handler_joystick, 0) == pdTRUE){
            printf("DEBUG: Joystick acionado.\n");
            add_msg(MSG_BUTTON_SEMAPHORO);
        } else if (xQueueReceive(queue_handle, bufferMsg, 0) == pdTRUE) {
            printf("DEBUG: Mensagem da fila recebida: %s\n", bufferMsg);
            add_msg(bufferMsg);
        } else if (xQueueReceive(mpu_queue_handle, &mpu_data, 0) == pdTRUE) {
            printf("DEBUG: Dados do MPU6050 recebidos.\n");
            char accel_msg[QUEUE_MSG_LENGHT];

            // Limpa a tela antes de exibir os novos dados do acelerômetro
            display_fill(false, false);

            display_msg(true, 1, 1, "::  TinyML  ::");

            // Formata e exibe o eixo X na linha 1
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ax: %.3f", mpu_data.accel_x_g);
            display_msg(false, 0, 3, accel_msg);

            // Formata e exibe o eixo Y na linha 3
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ay: %.3f", mpu_data.accel_y_g);
            display_msg(false, 0, 5, accel_msg);

            // Formata e exibe o eixo Z na linha 5
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Az: %.3f", mpu_data.accel_z_g);
            display_msg(false, 0, 7, accel_msg);

            // Atualiza o display com todos os novos dados de uma vez
            display_update();
        }

        // Reduzimos o delay para uma atualização mais fluida
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}