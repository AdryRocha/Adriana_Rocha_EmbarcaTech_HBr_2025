/**
/**
 * @file    main.c
 * @author  Adriana - Elias - Vagner
 * @brief
 * @version 0.1
 * @date
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "pico/cyw43_arch.h"

#include "include/button_a.h"
#include "include/button_b.h"
#include "include/button_j.h"
#include "include/config.h"
#include "include/ctrl.h"
#include "include/display_gate.h"
#include "include/led_rgb.h"
#include "include/mpu6050.h"

extern void mqtt_task(void *pvParameters);

int main(void)
{
    TaskHandle_t task_handle_button_a;
    TaskHandle_t task_handle_button_b;
    TaskHandle_t task_handle_button_j;
    TaskHandle_t task_handle_display_gate;
    TaskHandle_t task_handle_mpu6050;

    stdio_init_all();
    printf("Embarcatech Fase-2 :: Projeto Final\n");

    // Wi-Fi
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar Wi-Fi\n");
        for(;;) tight_loop_contents();
    }
    cyw43_arch_enable_sta_mode();          // <-- ADICIONE ESTA LINHA
        printf("Wi-Fi inicializado\n");

    const char *ssid = "ALEX_2G";
    const char *pass = "12345678";
    if (cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha ao conectar no Wi-Fi\n");
    } else {
        printf("Conectado no Wi-Fi!\n");
    }

    // Inicializa periféricos
    ctrl_init();
    led_rgb_init();
    mpu6050_init();

    // Cria tasks (inclua sua task de MQTT aqui também)
    xTaskCreate(button_a_task,     "ButtonA_Task",     512, NULL, 1, &task_handle_button_a);
    xTaskCreate(button_b_task,     "ButtonB_Task",     512, NULL, 1, &task_handle_button_b);
    xTaskCreate(button_j_task,     "ButtonJ_Task",     512, NULL, 1, &task_handle_button_j);
    xTaskCreate(display_gate_task, "DisplayGate_Task", 1024, NULL, 1, &task_handle_display_gate);
    xTaskCreate(mpu6050_task,      "MPU6050_Task",     512, NULL, 2, &task_handle_mpu6050);
    xTaskCreate(mqtt_task,         "MQTT",            4096, NULL, 1, NULL);

    // Scheduler
    vTaskStartScheduler();

    // Nunca deve chegar aqui
    for(;;) tight_loop_contents();
}