## Raspberry Pi Pico W - LED Embutido (Arquitetura Modular)

Este projeto demonstra a implementação de uma arquitetura de software modular para o controle do LED embutido na Raspberry Pi Pico W. O código é estruturado em camadas bem definidas (driver, HAL e aplicação), seguindo boas práticas de desenvolvimento de firmware.
__________________________________________________________________________________________________________________________________
### Objetivo do projeto

O principal objetivo deste projeto é exemplificar como estruturar um código embarcado seguindo uma arquitetura em camadas:

Camada de Driver: Acesso direto ao hardware (LED embutido da Pico W)

Camada HAL (Hardware Abstraction Layer): Camada de Abstração Interface simplificada e reutilizável

Camada de Aplicação: Lógica principal sem detalhes específicos de hardware

include/led_embutido.h e include/hal_led.h: As interfaces das respectivas camadas
__________________________________________________________________________________________________________________________________
### Componentes usados

Raspberry Pi Pico W

Microcontrolador RP2040 (dual-core ARM Cortex M0+ a 133MHz)

SDK da Raspberry Pi Pico

CMake para build  

Linguagem C

Módulo wireless CYW43439 (Wi-Fi e Bluetooth)

LED embutido conectado ao chip CYW43439 (CYW43_WL_GPIO_LED_PIN) 

Não são necessários componentes externos para este projeto, pois utilizamos apenas o LED embutido na própria placa.
__________________________________________________________________________________________________________________________________
### Pinagem dos dispositivos utilizados

Para este projeto, utilizamos apenas o LED embutido na placa Raspberry Pi Pico W:

Componente: LED embutido

Pino: CYW43_WL_GPIO_LED_PIN

Função: LED da interface Wi-Fi (GPIO virtual)

________________________________________________________________________________________________________________________________
### Resultados esperados ou obtidos

LED embutido da Pico W pisca a cada 500ms.

Arquitetura modular clara e reutilizável:

drivers/: interação direta com o hardware

hal/: funções reutilizáveis e desacopladas

app/: fluxo da aplicação principal

Códigos bem organizados para facilitar manutenção, testes e reuso em projetos futuros.


________________________________________________________________________________________________________________________________
### Estrutura do projeto
projeto/

├── app/

         └── main.c               # Aplicação principal

├── drivers/

         └── led_embutido.c       # Driver para acesso direto ao LED embutido

├── hal/

         └── hal_led.c            # Camada de abstração de hardware para LEDs

├── include/

         ├── led_embutido.h       # Interface do driver

         └── hal_led.h            # Interface da camada HAL

├── CMakeLists.txt           # Configuração de compilação

└── README.md                # Este arquivo

________________________________________________________________________________________________________________________________
### Licença

Este projeto está licenciado sob a MIT License.

________________________________________________________________________________________________________________________________
#### Este projeto foi desenvolvido como um exemplo educacional de arquitetura modular para sistemas embarcados.




