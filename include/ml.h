/**
 * @file    button_a.h
 * @author  Adriana - Elias - Vagner
 * @brief
 * @version 0.1
 * @date    08/09/2025
 */
#ifndef ML_H
#define ML_H

// Permite que este cabeçalho seja usado por C e C++
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Task Machine Learning (ML)
 * Recebe os sinais do acelerômetro e classifica o tipo de movimento e/ou anomalia
 */
void ml_task(void *pvParameters);

/**
 * @brief Get raw feature data
 * @param offset Offset in the feature vector
 * @param length Number of elements to read
 * @param out_ptr Pointer to the output buffer
 * @return Number of elements read
 */
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr);

#ifdef __cplusplus
}
#endif



#endif // ML_H
