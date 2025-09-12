/**
 * @file    button_a.c
 * @author  Adriana - Elias - Vagner
 * @brief   Task para o botão A
 * @version 0.1
 * @date
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "../include/button_a.h"
#include "../include/config.h"
#include "../include/ctrl.h"
#include "../include/led_rgb.h"
#include "../include/util.h"
#include "mpu6050.h"

#define BUTTON BUTTON_A_GPIO


/**
 * @brief Task do botão A - Inicializa GPIO do botão
 * Monitora o estado do botão, que quando pressionado, envia para o display,
 * via pilha, as acelerações dos três eixos, por 5 segundos.
 * @param
 */
void button_a_task(void *pvParameters){
    bool button_pressed = false;

    // Variáveis para a lógica de botão pressionado
    // Armazena o "timestamp" de quando o botão foi pressionado.
    TickType_t press_start_time = 0;
    //  Estrutura para guardar os dados lidos da fila do acelerômetro
    mpu6050_data_t mpu_data;
    // Array de caracteres para construir a mensagem que será enviada ao display
    char display_msg_buffer[QUEUE_MSG_LENGHT]; // Buffer para formatar a msg para o display

    // Obter o handle da fila do MPU6050
    QueueHandle_t mpu_queue_handle = (QueueHandle_t)mpu6050_get_queue();
    if (mpu_queue_handle == NULL) {
        printf("Button A Task: Erro ao obter a fila do MPU6050.\n");
        // Lidar com o erro, talvez deletando a tarefa (TO DO)
        vTaskDelete(NULL);
    }

    gpio_init(BUTTON);
    gpio_pull_up(BUTTON);
    // Delay para que o pull up faça efeito
    vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBONCE_MS));

    while(1){
        // Na BitDogLab, quando os botões (A e B) estão pressionados o I/O associado vai para nível baixo!
        if(!gpio_get(BUTTON)){  // Botao A pressionado!
            if(!button_pressed){
                button_pressed = true;
                // Registra o tempo de início
                press_start_time = xTaskGetTickCount();
                // Envia mensagem de controle de inicio para o gatekeeper
                util_gera_e_envia_msg(queue_handle, MSG_ACCEL_START);
                xSemaphoreTake(mutex_handler_led, portMAX_DELAY);
                //util_gera_e_envia_msg(queue_handle, MSG_BUTTON_A_LED_ON);
                led_rgb_set(LED_COR_RED);
            }
            // Lógica de envio de dados por 5 segundos
            // Verifica se ainda estamos dentro da janela de 5 segundos
            // press_start_time`: Armazena o "timestamp" de quando o botão foi pressionado.
            if (xTaskGetTickCount() - press_start_time < pdMS_TO_TICKS(5000)) {
                // Tenta receber um item da fila do MPU com um pequeno timeout ...
                // ... para não bloquear a tarefa indefinidamente
                if (xQueueReceive(mpu_queue_handle, &mpu_data, pdMS_TO_TICKS(20))) {
                    // Formata os dados de aceleração em uma string
                    snprintf(display_msg_buffer, QUEUE_MSG_LENGHT, "Ax:%.1f Ay:%.1f Az:%.1f",
                            mpu_data.accel_x_ms2,
                            mpu_data.accel_y_ms2,
                            mpu_data.accel_z_ms2);
                    // Envia a string formatada para a fila do display
                    xQueueSend(queue_handle, &display_msg_buffer, 0);
                }
            }
        }else { // Botão solto
            if(button_pressed){
                button_pressed = false;
                // Envia mensagem de FIM para o gatekeeper
                util_gera_e_envia_msg(queue_handle, MSG_ACCEL_STOP);
                led_rgb_set(LED_COR_BLACK);
                xSemaphoreGive(mutex_handler_led);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBONCE_MS));
    }
}
