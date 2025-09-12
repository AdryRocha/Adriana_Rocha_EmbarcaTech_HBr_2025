/**
 * @file    mpu6500.c
 * @author  Adriana - Elias - Vagner
 * @brief   Task que configura e lê os dados de aceleração dos
 * 3 eixos do MPU6500.
 * @version
 * @date    2025-09-08
 */

#include <stdio.h>
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "mpu6050.h"

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

// Fila para compartilhar dados do acelerômetro
QueueHandle_t mpu6050_queue = NULL;

// Semáforo para proteção do I2C
SemaphoreHandle_t i2c_mutex = NULL;

// Escala atual do acelerômetro (para conversão)
static uint8_t current_accel_scale = ACCEL_SCALE_2G;
static float scale_factor = 16384.0f; // ±2g default

void mpu6050_init(void) {
    // Inicializa I2C0 nos pinos GPIO0 (SDA) e GPIO1 (SCL)
    i2c_init(i2c0, 400 * 1000); // 400 kHz
    gpio_set_function(0, 3);    // GPIO0 como SDA | GPIO_FUNC_I2C = 3
    gpio_set_function(1, 3);    // GPIO1 como SCL | GPIO_FUNC_I2C = 3
    gpio_pull_up(0);            // Pull-up no SDA
    gpio_pull_up(1);            // Pull-up no SCL

    // Cria mutex para I2C
    i2c_mutex = xSemaphoreCreateMutex();

    // Cria fila para dados do acelerômetro
    mpu6050_queue = xQueueCreate(10, sizeof(mpu6050_data_t));

    // Configura o MPU6050
    if (xSemaphoreTake(i2c_mutex, portMAX_DELAY)) {
        // Acorda o MPU6050 (sai do modo sleep)
        uint8_t buf[2] = {MPU6050_PWR_MGMT_1, 0x00};
        i2c_write_blocking(i2c0, MPU6050_I2C_ADDR, buf, 2, false);

        // Configura escala do acelerômetro para ±2g
        uint8_t scale_buf[2] = {MPU6050_ACCEL_CONFIG, ACCEL_SCALE_2G};
        i2c_write_blocking(i2c0, MPU6050_I2C_ADDR, scale_buf, 2, false);

        xSemaphoreGive(i2c_mutex);
    }

    printf("MPU6050 inicializado (somente aceleração)\n");
}

void mpu6050_set_accel_scale(uint8_t scale) {
    if (xSemaphoreTake(i2c_mutex, portMAX_DELAY)) {
        uint8_t scale_buf[2] = {MPU6050_ACCEL_CONFIG, scale};
        i2c_write_blocking(i2c0, MPU6050_I2C_ADDR, scale_buf, 2, false);

        // Atualiza fator de escala para conversão
        switch (scale) {
            case ACCEL_SCALE_2G:
                scale_factor = 16384.0f;
                break;
            case ACCEL_SCALE_4G:
                scale_factor = 8192.0f;
                break;
            case ACCEL_SCALE_8G:
                scale_factor = 4096.0f;
                break;
            case ACCEL_SCALE_16G:
                scale_factor = 2048.0f;
                break;
            default:
                scale_factor = 16384.0f;
        }

        current_accel_scale = scale;
        xSemaphoreGive(i2c_mutex);
    }
}

bool mpu6050_read_accel_data(mpu6050_data_t *data) {
    uint8_t buffer[6]; // Apenas 6 bytes para aceleração (X, Y, Z)
    bool success = false;

    // Garante acesso exclusivo ao barramento (Mutex I2C)
    if (xSemaphoreTake(i2c_mutex, pdMS_TO_TICKS(100))) {
        // Lê 6 bytes começando do registrador ACCEL_XOUT_H
        uint8_t reg = MPU6050_ACCEL_XOUT_H;
        int result = i2c_write_blocking(i2c0, MPU6050_I2C_ADDR, &reg, 1, true);

        if (result == PICO_ERROR_GENERIC) {
            xSemaphoreGive(i2c_mutex);
            return false;
        }

        result = i2c_read_blocking(i2c0, MPU6050_I2C_ADDR, buffer, 6, false);

        xSemaphoreGive(i2c_mutex);

        if (result == PICO_ERROR_GENERIC) {
            return false;
        }

        // Processa os dados brutos de aceleração
        data->accel_x = (buffer[0] << 8) | buffer[1];
        data->accel_y = (buffer[2] << 8) | buffer[3];
        data->accel_z = (buffer[4] << 8) | buffer[5];

        // Converte para unidades g
        data->accel_x_g = data->accel_x / scale_factor;
        data->accel_y_g = data->accel_y / scale_factor;
        data->accel_z_g = data->accel_z / scale_factor;

       // Converte para unidades m/s²
        data->accel_x_ms2 = data->accel_x_g * CONVERT_G_TO_MS2;
        data->accel_y_ms2 = data->accel_y_g * CONVERT_G_TO_MS2;
        data->accel_z_ms2 = data->accel_z_g * CONVERT_G_TO_MS2;

        // Calcula magnitude da aceleração
        data->accel_magnitude_ms2 = sqrtf(
             data->accel_x_ms2 * data->accel_x_ms2 +
             data->accel_y_ms2 * data->accel_y_ms2 +
             data->accel_z_ms2 * data->accel_z_ms2
        );

        success = true;
    }

    return success;
}

void* mpu6050_get_queue(void) {
    return mpu6050_queue;
}

void mpu6050_task(void *pvParameters) {
    mpu6050_data_t data;
    TickType_t last_wake_time = xTaskGetTickCount();
    uint32_t read_count = 0;

    for (;;) {
        if (mpu6050_read_accel_data(&data)) {
            // Envia dados para a fila (se houver consumidores) | Produção
            xQueueSend(mpu6050_queue, &data, 0);

            //  Saida para monitor serial
            /*
            printf("%.1f \t", data.accel_x_ms2);
            printf("%.1f \t", data.accel_y_ms2);
            printf("%.1f \t", data.accel_z_ms2);
            printf("\n");
            */

        } else {
            printf("MPU6050 read error\n");
        }
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(10));
    }
}