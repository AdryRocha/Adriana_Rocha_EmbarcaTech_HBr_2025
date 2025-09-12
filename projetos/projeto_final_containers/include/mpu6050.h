#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include <stdbool.h>
#include "mpu6050.h"

// Endereço I2C do MPU6050
#define MPU6050_I2C_ADDR 0x68

// Registros do MPU6050 (apenas aceleração)
#define MPU6050_WHO_AM_I 0x75
#define MPU6050_PWR_MGMT_1 0x6B     // Controle de energia
#define MPU6050_ACCEL_CONFIG 0x1C   // Configuração de escala
#define MPU6050_ACCEL_XOUT_H 0x3B   // Dados Aceleração X (high)

// Escalas de aceleração
#define ACCEL_SCALE_2G 0x00     // ±2g  | 16384 LSB/g
#define ACCEL_SCALE_4G 0x08     // ±4g  |  8192 LSB/g
#define ACCEL_SCALE_8G 0x10     // ±8g  |  4096 LSB/g
#define ACCEL_SCALE_16G 0x18    // ±16g |  2048 LSB/g

// Conversão de g para m/s²
#ifndef CONVERT_G_TO_MS2
#define CONVERT_G_TO_MS2 9.80665f
#endif

// Estrutura para dados do acelerômetro apenas para aceleração
typedef struct {
    // Valores brutos do sensor | -32768 a +32767
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    // Dados convertidos ±2g, ±4g, etc... (accel_x_g = accel_x / scale_factor)
    float accel_x_g;
    float accel_y_g;
    float accel_z_g;

    float accel_x_ms2;  // Dados em m/s²
    float accel_y_ms2;
    float accel_z_ms2;
    
    // Magnitude vetorial total é um valor escalar que representa a intensidade total do vetor aceleração em um
    // ponto 3D,independentemente de sua direção. É calculada usando o Teorema de Pitágoras em três dimensões
    // accel_magnitude = √(accel_x_g² + accel_y_g² + accel_z_g²)
    float accel_magnitude_ms2;
} mpu6050_data_t;

// Protótipos das funções
/**
 * @brief
    1. Configura hardware I2C
    2. Cria mutex para acesso thread-safe
    3. Cria fila para compartilhamento de dados
    4. Configura MPU6050 (sai do sleep mode)
    5. Define escala padrão (±2g)
 */
void mpu6050_init(void);

/**
 * @brief
    1. Acquire mutex (timeout 100ms)
    2. Escreve registrador de início (0x3B)
    3. Lê 6 bytes consecutivos (X, Y, Z)
    4. Release mutex
    5. Converte dados brutos para g
    6. Calcula magnitude
    7. Retorna sucesso/erro
    8. Taxa de Amostragem:
        - 100 Hz (10ms entre leituras)
        - 6 bytes por leitura (apenas aceleração)
        - ≈600 bytes/s no barramento I2C
    9. Latência:
        - ≤10ms entre leituras (garantido por vTaskDelayUntil)
        - ≈1ms para leitura e processamento
    10. Consumo de Recursos:
        - 512 bytes de stack para a task
        - 40 bytes por mensagem na fila
        - 10 mensagens na fila = 400 bytes RAM
 */
bool mpu6050_read_accel_data(mpu6050_data_t *data);

/**
 * @brief
 1. Configura escala: 2, 4, 8 e 16g
 2. Valor padrão: 2g
*/
void mpu6050_set_accel_scale(uint8_t scale);

/**
 * @brief
1. Lê dados do sensor
2. Envia para fila (se houver consumidores)
3. Debug periódico (a cada 100 leituras)
4. Delay preciso de 10ms (100Hz)
 */
void mpu6050_task(void *pvParameters);

/**
*@brief
1. Acesso a fila

*/
void* mpu6050_get_queue(void);

#endif // MPU6050_H