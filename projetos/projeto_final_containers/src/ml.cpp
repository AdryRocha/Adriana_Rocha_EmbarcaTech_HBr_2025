/**
 * @file    ml.cpp
 * @author  Adriana - Elias - Vagner
 * @brief   Task Machine Learning (ML)
 * Classifica os movimentos e/ou anomalias em função dos sinais do acelerômetro.
 * @version
 * @date    2025-08-09
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "ml.h"

// Trate todas as funções declaradas neste arquivo como funções C puras, sem modificar seus nomes ("name mangling")
extern "C"{
#include "mpu6050.h"
}

#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

// Buffer para armazenar os dados do acelerômetro em tempo real
// O tamanho é definido pelo SDK do Edge Impulse com base no  modelo treinado
static float features_buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

// Prototipo das funções
void print_inference_result(ei_impulse_result_t result);
void log_top_inference_result(ei_impulse_result_t result, float threshold);

void ml_task(void *pvParameters) {
    printf("ML Task iniciada ...\n");

    // Defina seu limite de confiança para inferência
    const float CONFIDENCE_THRESHOLD = 0.75f; // 75%

    // Obtem o handle (manipulador) da fila do MPU6500
    QueueHandle_t mpu_queue_handle = (QueueHandle_t)mpu6500_get_queue();

    if (mpu_queue_handle == NULL) {
        printf("ML Task: Erro ao obter a fila do MPU6500.\n");
        vTaskDelete(NULL); // Deleta a tarefa se a fila não existir
    }

    // Estrutura para receber os dados da fila
    mpu6500_data_t mpu_data;

    // Variável que conta quantas amostras já foram coletadas
    int samples_collected = 0;

    /* Loop infinito:
    - Orientado a eventos, não utiliza loop de atraso (vTaskDelay).
    - Espera por um novo dado da fila.
    - Adiciona o dado ao buffer.
    - Quando o buffer estiver cheio, invoca o classificador do Edge Impulse.
    - Imprime o resultado.
    - Limpa o buffer para começar a coletar a próxima janela de dados.
     */
    for (;;) {
        // Espera por dados da fila do acelerômetro
        // A tarefa ficará bloqueada aqui até um item chegar, sem consumir CPU
        // A função xQueueReceive é "bloqueante"
        if (xQueueReceive(mpu_queue_handle, &mpu_data, portMAX_DELAY) == pdPASS) {
            // Preenche o buffer com os dados recebidos (eixo X, Y, Z)
            features_buffer[samples_collected * EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME + 0] = mpu_data.accel_x_ms2;
            features_buffer[samples_collected * EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME + 1] = mpu_data.accel_y_ms2;
            features_buffer[samples_collected * EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME + 2] = mpu_data.accel_z_ms2;

            samples_collected++;

            // Verifica se o buffer (janela) está cheio
            if (samples_collected >= EI_CLASSIFIER_RAW_SAMPLE_COUNT) {
                printf("Coletou %d amostras, executando inferencia...\n", samples_collected);

                ei_impulse_result_t result = {nullptr};
                signal_t features_signal;
                features_signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
                features_signal.get_data = &raw_feature_get_data;

                // Executa o classificador
                EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false /* debug */);
                if (res != EI_IMPULSE_OK) {
                    ei_printf("ERR: Failed to run classifier (%d)\n", res);
                } else {
                    // Imprime todos os resultados
                    print_inference_result(result);

                    // Imprime apenas o melhor resultado
                    log_top_inference_result(result, CONFIDENCE_THRESHOLD);
                }
                // Limpa o contador para começar a coletar a próxima janela
                samples_collected = 0;
            }
        }
    }
}

// Wrapper (encapsulador) para que o SDK do Edge Impulse possa ler do buffer
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
  // Copia bloco de memória (Destino, Origem, Tamanho)
  memcpy(out_ptr, features_buffer + offset, length * sizeof(float));
  return 0;
}

/**
 * @brief Função do Edge Impulse que apresenta o resultado da inferência
 * @param ei_impulse_result_t estrutura com os resultados da inferência
 * @return Classes com seus respectivos valores de confiança
 */
void print_inference_result(ei_impulse_result_t result) {
    // Print how long it took to perform inference
    ei_printf("Timing: DSP %d ms, inference %d ms, anomaly %d ms\r\n",
            result.timing.dsp,
            result.timing.classification,
            result.timing.anomaly);

    // Print the prediction results
    ei_printf("Predictions:\r\n");
    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        ei_printf("  %s: ", ei_classifier_inferencing_categories[i]);
        ei_printf("%.5f\r\n", result.classification[i].value);
    }

    // Print anomaly result (if it exists)
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("Anomaly prediction: %.3f\r\n", result.anomaly);
#endif
}

/**
 * @brief Encontra a classe com maior pontuação, verifica contra um threshold e
 * imprime o resultado.
 *
 * @param result O resultado da inferência do Edge Impulse.
 * @param threshold O limite de confiança (ex: 0.75 para 75%).
 */
void log_top_inference_result(ei_impulse_result_t result, float threshold) {
    // 1. Variáveis para encontrar o maior valor
    float max_value = 0.0f;
    int max_index = -1;

    // 2. Loop para encontrar a classe com a maior pontuação
    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        if (result.classification[i].value > max_value) {
            max_value = result.classification[i].value;
            max_index = i;
        }
    }

    // 3. Verifica se a maior pontuação atende ao limite de confiança
    if (max_index != -1 && max_value >= threshold) {
        // Se a confiança for alta o suficiente, imprime a classe
        ei_printf("Classe Inferida: %s (Confianca: %.5f)\n",
                  ei_classifier_inferencing_categories[max_index],
                  max_value);
    } else if (max_index != -1) {
        // Se a confiança for muito baixa, informa que o resultado foi descartado
        ei_printf("Resultado incerto: %s (Confianca: %.5f) - Abaixo do limite de %.2f\n",
                  ei_classifier_inferencing_categories[max_index],
                  max_value,
                  threshold);
    } else {
        // Caso nenhuma classificação seja encontrada
        ei_printf("Nenhum resultado de classificacao encontrado.\n");
    }
}
