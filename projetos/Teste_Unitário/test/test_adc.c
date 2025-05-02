#include "unity.h"
#include "adc_utils.h"

void setUp(void) {} // Função chamada antes de cada teste, pode ser usada para inicializar variáveis ou configurar o ambiente de teste
void tearDown(void) {} // Função chamada após cada teste, pode ser usada para limpar recursos ou restaurar o estado

void test_adc_to_celsius(void) { // Função de teste para verificar a função adc_to_celsius
    // Para 0.706V, o valor do ADC é (0.706 / 3.3) * 4095 ≈ 876
    uint16_t adc_val = 876; // Definimos o valor de ADC simulado como 876
    float expected = 27.0f; // A temperatura esperada para 0.706V é 27°C, conforme a fórmula
    // Chama a função adc_to_celsius com o valor de ADC simulado e armazena o resultado
    float actual = adc_to_celsius(adc_val); // Chama a função adc_to_celsius com o valor de ADC simulado e armazena o resultado
    // Verifica se o valor retornado pela função está dentro do esperado
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected, actual); // TEST_ASSERT_FLOAT_WITHIN permite uma margem de erro de 0.1°C devido à precisão de ponto flutuante
    // 0.1f é a margem de erro, expected é o valor esperado, actual é o valor calculado
}

int main(void) { // Função principal para executar os testes unitários
    UNITY_BEGIN(); // Inicia o framework Unity para rodar os testes
    RUN_TEST(test_adc_to_celsius);  // Executa o teste test_adc_to_celsius definido acima
    // Finaliza o framework Unity e retorna o resultado dos testes
    return UNITY_END(); // Retorna 0 se todos os testes passaram, ou um valor diferente se houve falhas
}
// O código acima define uma função para converter valores de ADC em temperatura em Celsius e inclui testes unitários para verificar a precisão dessa conversão.