# Teste Unitário - Raspberry PI Pico W

O ADC interno da Raspberry Pi Pico W pode ser usado para medir a temperatura através de um canal especial (sensor embutido). O valor lido do ADC (12 bits) deve ser convertido para graus Celsius usando a fórmula da documentação oficial:

![image](https://github.com/user-attachments/assets/b7cee2db-3f79-471a-8ff4-db4d8a567652)

Essa função converte a tensão lida do sensor de temperatura para Celsius, assumindo Vref = 3.3V e resolução de 12 bits.





______________________________________________________________________________________________________________________
## Objetivo
1. Implemente a função float adc_to_celsius(uint16_t adc_val);

2. Escreva uma função de teste unitário que verifica se a função retorna o valor correto (com margem de erro) para uma leitura simulada de ADC. Sugere-se o uso da biblioteca Unity para o teste unitário.

3. Use um teste com valor de ADC conhecido (ex.: para 0.706 V, a temperatura deve ser 27 °C).
______________________________________________________________________________________________________________________
## Componentes usados

- Raspberry Pi Pico W
- Sensor de temperatura interno do RP2040
- Compilador `arm-none-eabi-gcc`
- CMake + Ninja
- Biblioteca Unity para testes unitários
- VS Code com extensões Pico SDK

______________________________________________________________________________________________________________________

## Pinagem dos dispositivos utilizados
Este projeto não utiliza conexões externas. A leitura é feita via ADC interno do microcontrolador para o sensor de temperatura embutido.

______________________________________________________________________________________________________________________

### Estrutura

Teste_Unitario/

├── build/

├── include/

          └── adc_utils.h          (declaração da função adc_to_celsius, permitindo que outros arquivos a usem sem 
                                    conhecer sua implementação.)
                                 
├── src/

          └── adc_utils.c          (implementação da função - Converte um valor digital (0 a 4095) do ADC para uma tensão entre 
                                    0V e 3.3V:

                                    \text{voltage} = \text{adc_val} \times \frac{3.3}{4095}
                                    Converte a tensão para temperatura usando fórmula fornecida pelo datasheet do RP2040:)
                                       
                                    temperatura (°C) = 27 - (V-0.706)/0.001721
                                       
                                    Isso é baseado no sensor de temperatura interno do RP2040.
                                       
├── unity/

          ├── unity.c              (biblioteca Unity)

          ├── unity.h              

          └── unity_internals.h    

├── test/

          └── test_adc.c           (arquivo com os testes unitários - Testa a função com um valor de ADC conhecido (876) 
                                    que representa 0.706V.

                                   Valida se o resultado da conversão é 27 °C, com margem de erro de ±0.1.

                                       UNITY_BEGIN() e UNITY_END() são funções da biblioteca Unity que inicializam e 
                                       finalizam a execução dos testes.)

├── CMakeLists.txt

___________________________________________________________________________________________________________________________

### Resultados esperados ou obtidos
- O valor de ADC 876 representa 0.706V.

- Espera-se que a conversão produza 27.0 °C.

- O teste unitário confere se o valor está dentro da margem de erro de 0.1 °C.

- O projeto é uma base prática para desenvolvimento embarcado com boas práticas de testes automáticos.

___________________________________________________________________________________________________________________________
### Licença
Este projeto está licenciado sob a MIT License. 


