# Sintetizador de Áudio com Raspberry Pi Pico

O objetivo do projeto é desenvolver um sintetizador de áudio utilizando a placa BitDogLab. A tarefa consiste em capturar um sinal de áudio analógico pelo microfone, convertê-lo para o formato digital usando o conversor ADC, armazenar temporariamente esses dados na memória RAM e, por fim, reproduzir o som através de um buzzer usando modulação por largura de pulso (PWM). O controle das funções de gravação e reprodução deve ser feito por botões , com feedback visual fornecido por um LED RGB  e a visualização da forma de onda em um display OLED

## Funcionalidades

* **Gravação de Áudio:** Captura áudio mono a uma taxa de amostragem de 8000 Hz.
* **Reprodução de Áudio:** Reproduz o áudio gravado através de um sinal PWM em um buzzer.
* **Visualização em Tempo Real:** Exibe a forma de onda do áudio gravado em um display OLED de 128x64 pixels.
* **Interface Simples:** Utiliza dois botões para controle (Gravar e Reproduzir) e um LED bicolor para indicar o status atual.

## Hardware Necessário

| Componente                | Detalhes                                        |
| ------------------------- | ----------------------------------------------- |
| **Microcontrolador** | Raspberry Pi Pico ou Pico W                     |
| **Display** | Módulo OLED 128x64 I2C (controlador SSD1306)    |
| **Sensor de Som** | Módulo Microfone MAX9814 ou similar (saída analógica) |
| **Saída de Som** | Buzzer Passivo                                  |
| **Entrada do Usuário** | 2x Botões (Push-buttons)                        |
| **Feedback Visual** | 1x LED RGB (ou dois LEDs de cores diferentes)   |
| **Componentes Passivos** | Breadboard e fios de jumper                     |

## Diagrama de Conexões (Pinagem)

Use os fios de jumper para conectar os componentes ao seu Raspberry Pi Pico conforme a tabela abaixo.

| Componente          | Pino do Componente | Pino do Raspberry Pi Pico | Descrição                                  |
| ------------------- | ------------------ | ------------------------- | ------------------------------------------ |
| **Microfone** | `OUT`              | `GP26` (ADC0)             | Saída analógica do microfone               |
|                     | `VCC`              | `3V3`                     | Alimentação do módulo                      |
|                     | `GND`              | `GND`                     | Terra                                      |
| **Buzzer Passivo** | `+`                | `GP21`                    | Sinal PWM para gerar o som                 |
|                     | `-`                | `GND`                     | Terra                                      |
| **Display OLED I2C**| `SDA`              | `GP0` (I2C0 SDA)          | Linha de dados I2C                         |
|                     | `SCL`              | `GP1` (I2C0 SCL)          | Linha de clock I2C                         |
|                     | `VCC`              | `3V3`                     | Alimentação do display                     |
|                     | `GND`              | `GND`                     | Terra                                      |
| **Botão A (Gravar)**| Perna 1            | `GP5`                     | Conectado ao pino de entrada               |
|                     | Perna 2            | `GND`                     | Conectado ao terra                         |
| **Botão B (Tocar)** | Perna 1            | `GP6`                     | Conectado ao pino de entrada               |
|                     | Perna 2            | `GND`                     | Conectado ao terra                         |
| **LED** | Cátodo Vermelho    | `GP15`                    | Controla a luz vermelha (gravação)         |
|                     | Cátodo Verde       | `GP16`                    | Controla a luz verde (reprodução)          |
|                     | Anodo Comum        | `3V3` (via resistor)      | Se for ânodo comum, ou `GND` se for cátodo comum |

**Nota:** Os pinos de botões `GP5` e `GP6` são configurados com resistores de pull-up internos, então não são necessários resistores externos.

## Configuração do Ambiente de Desenvolvimento

Para compilar e carregar este projeto, você precisará ter o **SDK do Raspberry Pi Pico (C/C++)** configurado em sua máquina.

1.  **Clone o Repositório:**
    ```bash
    git clone [https://github.com/seu-usuario/sintetizador_audio.git](https://github.com/seu-usuario/sintetizador_audio.git)
    cd sintetizador_audio
    ```

2.  **Configure o SDK do Pico:**
    Siga as instruções oficiais em [Getting started with Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) para instalar o toolchain (ARM GCC, CMake, etc.) e o SDK.

3.  **Defina o Caminho do SDK:**
    Exporte a variável de ambiente `PICO_SDK_PATH` para que o CMake encontre o SDK.
    ```bash
    export PICO_SDK_PATH=/caminho/para/o/seu/pico-sdk
    ```

## Como Compilar e Carregar

1.  **Crie um diretório de build:**
    ```bash
    mkdir build
    cd build
    ```

2.  **Execute o CMake para configurar o projeto:**
    ```bash
    cmake ..
    ```

3.  **Compile o projeto com o Make:**
    ```bash
    make
    ```

4.  **Carregue o Firmware:**
    * Pressione e segure o botão `BOOTSEL` do seu Pico e conecte-o ao computador.
    * Ele será montado como um dispositivo de armazenamento USB.
    * Arraste e solte o arquivo `sintetizador_audio.uf2` (que está dentro da pasta `build`) para o Pico.
    * O Pico reiniciará automaticamente e o programa começará a ser executado.

## Como Usar

1.  **Estado Inicial:** Ao ligar, o sistema fica em modo de espera (LED apagado).
2.  **Gravar:** Pressione o **Botão A (GP5)**. O LED ficará **vermelho**, e o sistema gravará 2 segundos de áudio. Após a gravação, a forma de onda será exibida no display OLED.
3.  **Reproduzir:** Pressione o **Botão B (GP6)**. O LED ficará **verde**, e o áudio gravado será reproduzido no buzzer.
4.  Você pode gravar um novo áudio a qualquer momento pressionando o Botão A novamente.

## Estrutura do Código

| Arquivo               | Descrição                                                                              |
| --------------------- | -------------------------------------------------------------------------------------- |
| `main.c`              | Ponto de entrada do programa. Contém a lógica principal, o loop de estados e o tratamento de botões. |
| `audio.c` / `.h`      | Módulo para captura de áudio do microfone (via ADC) e reprodução no buzzer (via PWM).  |
| `display.c` / `.h`    | Módulo para controle do display OLED (inicialização, limpeza e desenho da forma de onda). |
| `utils.c` / `.h`      | Funções de utilidade para inicializar e controlar os botões e LEDs.                   |
| `CMakeLists.txt`      | Arquivo de build do CMake que define as fontes, bibliotecas e configurações do projeto.   |
| `lib/pico-ssd1306/`   | Biblioteca de terceiros para controlar o display OLED SSD1306.                         |
