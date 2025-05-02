#include "adc_utils.h" // Inclui o cabeçalho adc_utils.h que contém a declaração da função adc_to_celsius

float adc_to_celsius(uint16_t adc_val) {
    float voltage = adc_val * 3.3f / 4095.0f; // Converte o valor do ADC para tensão
    // Aplica a fórmula de temperatura: T = 27 - (voltage - 0.706) / 0.001721 
    // Subtrai 0.706 da tensão (valor de referência da fórmula)
    // Divide o resultado por 0.001721 (constante da fórmula que representa a variação de tensão por grau Celsius)
    // Subtrai o resultado de 27 para obter a temperatura em Celsius
    // Retorna o valor da temperatura calculada
    // 27.0f é a temperatura de referência em Celsius
    // voltage - 0.706f é a diferença entre a tensão medida e a tensão de referência
    // 0.001721f é a constante que relaciona a variação de tensão com a variação de temperatura
    // A fórmula final calcula a temperatura em Celsius com base na tensão medida
    // A fórmula é derivada da relação entre a tensão e a temperatura do sensor
    // A fórmula é baseada na relação linear entre a tensão e a temperatura do sensor
    return 27.0f - (voltage - 0.706f) / 0.001721f;  
}
