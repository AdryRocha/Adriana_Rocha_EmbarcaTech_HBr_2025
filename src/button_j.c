/**
 * @file    button_j.c
 * @author  Adriana - Elias - Vagner
 * @brief   Task para o botão J: Joystick
 * @version 0.1
 * @date    2025-08-25
 */
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "../include/button_j.h"
#include "../include/config.h"
#include "../include/ctrl.h"

#define BUTTON BUTTON_J_GPIO

/**
 * @brief Inicializa GPIO do botão C Joystick
 * Monitora transições de estado do,botão :
 *   - quando pressionado gera um semáforo
 */
void button_j_task(void *pvParameters){
    bool button_pressed = false;

    gpio_init(   BUTTON);
    gpio_pull_up(BUTTON);
    vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBONCE_MS)); // Delay para que o pull up faça efeito

    while(1){
        if(!gpio_get(BUTTON)){
            if(!button_pressed){
                button_pressed = true;
                xSemaphoreGive(semaphored_handler_joystick);
                // msg "BA PRESS"
                // msg "BA RELEASE" 12 123
                // msg "BA LED ON"

                //xSemaphoreTake(mutex_handler_led, portMAX_DELAY);
                // msg
                //led_rgb_set(LED_COR_GREEN);
            }
        }else{
            if(button_pressed){
                button_pressed = false;
                // msg

                //led_rgb_set(LED_COR_BLACK);
                //xSemaphoreGive(mutex_handler_led);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBONCE_MS));
    }
}
