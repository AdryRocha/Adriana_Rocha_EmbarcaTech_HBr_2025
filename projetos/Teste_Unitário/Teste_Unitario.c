#include <stdint.h> // Inclui a biblioteca para tipos de dados inteiros padrão, como uint16_t
#include "unity.h" // Inclui a biblioteca Unity para testes unitários
#include "adc_utils.h" // Inclui o cabeçalho do módulo adc_utils.h


// Bloco condicional para testes unitários, ativado apenas se TEST estiver definido
#ifdef TEST
#include "unity.h" // Inclui a biblioteca Unity para testes unitários

// Função de teste para verificar a função adc_to_celsius
void test_adc_to_celsius(void) {
    // Para 0.706V, o valor do ADC é (0.706 / 3.3) * 4095 ≈ 876
    // Definimos o valor de ADC simulado como 876
    uint16_t adc_val = 876;
    
    // A temperatura esperada para 0.706V é 27°C, conforme a fórmula
    float expected = 27.0f;
    
    // Chama a função adc_to_celsius com o valor de ADC simulado e armazena o resultado
    float actual = adc_to_celsius(adc_val);
    
    // Verifica se o valor retornado pela função está dentro do esperado
    // TEST_ASSERT_FLOAT_WITHIN permite uma margem de erro de 0.1°C devido à precisão de ponto flutuante
    // 0.1f é a margem de erro, expected é o valor esperado, actual é o valor calculado
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected, actual);
}

// Funções obrigatórias do Unity (mesmo que fiquem vazias)
void setUp(void) {}
void tearDown(void) {}

// Função principal para executar os testes unitários
int main(void) {
    // Inicia o framework Unity para rodar os testes
    UNITY_BEGIN();
    
    // Executa o teste test_adc_to_celsius definido acima
    RUN_TEST(test_adc_to_celsius);
    
    // Finaliza o framework Unity e retorna o resultado dos testes
    // Retorna 0 se todos os testes passaram, ou um valor diferente se houve falhas
    return UNITY_END();
}
#endif
// O código acima define uma função para converter valores de ADC em temperatura em Celsius e inclui testes unitários para verificar a precisão dessa conversão.