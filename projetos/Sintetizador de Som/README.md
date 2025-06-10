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
___________________________________________________________________________________________

Etapas de Desenvolvimento do Projeto

1. Aquisição do Sinal do Microfone (via ADC)
Esta etapa é implementada na função init_adc() e parcialmente na capture_audio() do arquivo audio.c.

Código Correspondente: A função init_adc() configura o hardware. Ela inicializa o periférico (adc_init()), habilita o pino ADC_MIC_IN_PIN (GP26) para uso com o ADC (adc_gpio_init) e seleciona o canal ADC_INPUT (ADC0) para leitura.
Teste (Conceitual): Para testar, você criaria um laço while(true) no main que chama adc_read() repetidamente e imprime o resultado com printf. Isso permite observar a variação dos valores numéricos que o microfone está captando.

____________________________________________________________________________________________
2. Armazenamento dos Dados de ADC num Buffer
Esta etapa é a principal funcionalidade da função capture_audio(uint16_t *buffer, size_t size) em audio.c.

Código Correspondente: A função recebe um ponteiro para buffer e seu size. Dentro dela, um laço for (size_t i = 0; i < size; i++) é usado como contador. Em cada iteração, buffer[i] = adc_read(); armazena a leitura do microfone diretamente no array, exatamente como a etapa descreve.
Teste de Verificação: O código if (i % 1000 == 0) com um printf dentro do laço atua como uma verificação parcial, mostrando que a captura está progredindo. Para um teste completo, você poderia adicionar um segundo laço no main após a chamada de capture_audio para imprimir todo o conteúdo do buffer.

____________________________________________________________________________________________
3. Configuração da Taxa de Amostragem
A taxa de amostragem é definida pela constante SAMPLE_RATE e implementada com um sleep_us em audio.c.

Código Correspondente: No topo de audio.c, a constante #define SAMPLE_RATE 8000 define a frequência desejada (8 kHz). Dentro do laço de captura da função capture_audio, a linha sleep_us(1000000 / SAMPLE_RATE); implementa essa taxa. Com SAMPLE_RATE sendo 8000, isso resulta em uma pausa de exatamente 125 microssegundos entre cada amostra.

____________________________________________________________________________________________
4. Configuração do Período de Gravação
Esta configuração é feita no arquivo main.c através de constantes que definem o tamanho do buffer.

Código Correspondente: No main.c que preparei para você, as constantes #define AUDIO_DURATION_S 2, #define SAMPLE_RATE 8000 e #define BUFFER_SIZE (AUDIO_DURATION_S * SAMPLE_RATE) realizam exatamente o que a etapa pede. O buffer uint16_t audio_buffer[BUFFER_SIZE]; é então declarado com o tamanho calculado (2 segundos * 8000 Hz = 16000 amostras).

____________________________________________________________________________________________
5. Manipulação do Sinal do PWM
Esta conversão é o núcleo da função play_audio() em audio.c.

Código Correspondente: A função init_pwm() configura o PWM_AUDIO_OUT_PIN (GP21) e, crucialmente, define o valor máximo do contador PWM com pwm_config_set_wrap(&config, 250). Isso significa que a resolução do PWM é de 0 a 250.
Dentro de play_audio(), a linha uint16_t level = (uint16_t)(((float)buffer[i] / 4095.0f) * 250.0f); faz o ajuste necessário: converte o valor do ADC (0-4095) para a escala do PWM (0-250), convertendo os dados em um sinal audível. A função pwm_set_gpio_level(PWM_AUDIO_OUT_PIN, level); envia esse sinal para o buzzer.

___________________________________________________________________________________________
6. Reprodução Completa do Áudio Armazenado
A função play_audio() em audio.c também garante a reprodução na velocidade correta.

Código Correspondente: O laço for (size_t i = 0; i < size; i++) serve como o contador que garante que todos os valores do buffer sejam enviados. Dentro deste laço, a linha sleep_us(1000000 / SAMPLE_RATE); usa a mesma pausa da captura, garantindo que o áudio seja reproduzido na mesma frequência (e, portanto, mesma velocidade e tom) em que foi gravado. O teste final é realizado no main.c, que chama capture_audio e depois play_audio.

____________________________________________________________________________________________
7. Controle das Funções com os Botões
Esta funcionalidade é implementada no main.c e utiliza as funções de utils.c.

Código Correspondente: O arquivo utils.c define BUTTON_A_PIN (GP5) e BUTTON_B_PIN (GP6) e fornece a função bool button_pressed(uint gpio). O main.c utiliza uma máquina de estados (enum AppState) e, dentro do laço while(true), chama button_pressed() para verificar se os botões foram acionados, mudando o estado do programa de IDLE para RECORDING ou PLAYING.

____________________________________________________________________________________________
8. Retorno Visual de Ativação (com LEDs)
O controle dos LEDs também está no main.c e usa as funções de utils.c.

Código Correspondente: O arquivo utils.c define RED_LED_PIN (GP15) e GREEN_LED_PIN (GP16) e a função void set_led_color(int color). No main.c, dentro da máquina de estados, set_led_color(RED); é chamado no estado STATE_RECORDING e set_led_color(GREEN); é chamado no estado STATE_PLAYING, fornecendo o feedback visual exato que a etapa descreve.

____________________________________________________________________________________________
9. Visualização da Forma de Onda no Display OLED
Esta etapa é implementada pela função show_waveform() no arquivo display.c.

Código Correspondente: A função show_waveform primeiro limpa o display (ssd1306_clear). Em seguida, ela percorre as primeiras 128 amostras do buffer. Para cada amostra, a linha uint8_t height = (buffer[i] >> 6); converte a altura da amostra para a escala do display (0-63) de forma eficiente (divisão por 64). A função ssd1306_draw_line() desenha a coluna de pixels correspondente.

____________________________________________________________________________________________
10. Aprimore a Saída de Áudio (Opcional)
Isso corresponde a uma alteração de hardware.

Aplicação no Projeto: O pino PWM_AUDIO_OUT_PIN (GP21), que atualmente está conectado ao buzzer, seria conectado à entrada de um módulo amplificador Classe-D. O código não precisaria de alterações, mas o resultado sonoro seria muito superior.
11. Refine o Processamento de Áudio (Opcional)
Isso corresponde a uma alteração de software, especificamente na função capture_audio.

Aplicação no Projeto: Para implementar um filtro de média móvel, você modificaria o laço em capture_audio. Em vez de simplesmente buffer[i] = adc_read();, você faria algo como:
C

// (Fora do loop) uint16_t ultima_amostra = 0;
// (Dentro do loop)
uint16_t amostra_atual = adc_read();
buffer[i] = (amostra_atual + ultima_amostra) / 2; // Média de 2 pontos
ultima_amostra = amostra_atual;
Isso reduziria o ruído de alta frequência durante a própria captura.

____________________________________________________________________________________________
Perguntas da Reflexão Final

1. Quais técnicas de programação podemos usar para melhorar a gravação e a reprodução do áudio?
Para melhorar a qualidade da gravação e reprodução, podemos ir além da implementação básica e aplicar técnicas de programação mais avançadas que otimizam o uso do hardware e processam o sinal de áudio.
____________________________________________________________________________________________
Uso de Interrupções e DMA (Acesso Direto à Memória):

Interrupções de Timer: Em vez de usar um delay (pausa) dentro de um loop para controlar a taxa de amostragem, podemos configurar um timer de hardware para gerar uma interrupção em intervalos precisos (ex: a cada 125µs para uma taxa de 8kHz). Dentro da rotina de serviço da interrupção (ISR), faríamos a leitura do ADC. Isso garante uma taxa de amostragem perfeitamente constante, eliminando o "jitter" (variação no tempo) que degrada a qualidade do áudio.
DMA (Direct Memory Access): O DMA é um recurso que permite que periféricos (como o ADC) transfiram dados diretamente para a memória RAM sem a intervenção da CPU. Podemos configurar o DMA para, a cada vez que uma conversão do ADC estiver pronta, mover o resultado para o próximo espaço livre no nosso buffer de áudio. Isso libera a CPU para realizar outras tarefas, como atualizar o display, verificar botões ou até mesmo aplicar filtros no áudio em tempo real, resultando em um sistema mais eficiente e com menor chance de perder amostras.

____________________________________________________________________________________________
Filtragem Digital de Sinais (DSP):

Os documentos mencionam a possibilidade de filtrar o ruído. Uma técnica comum é aplicar um filtro passa-baixas (Low-Pass Filter) por software. O microfone pode captar ruídos de alta frequência (chiados) que não fazem parte do som de interesse (como a voz). Um filtro passa-baixas remove essas frequências indesejadas, resultando em um som mais limpo.

Outra técnica é a normalização. Após a gravação, podemos varrer todo o buffer para encontrar o valor de pico. Em seguida, multiplicamos todas as amostras por um fator que faça o valor de pico corresponder à amplitude máxima do nosso sistema de reprodução (o nível máximo do PWM). Isso garante que o volume seja sempre alto e claro, aproveitando toda a faixa dinâmica do buzzer.

____________________________________________________________________________________________
Técnica de Double Buffering (Ping-Pong Buffer):

Esta técnica é ideal para processamento em tempo real. Em vez de um único buffer de áudio, usamos dois. Enquanto o DMA está preenchendo um buffer (o "ping") com novas amostras do microfone, a CPU pode trabalhar no outro buffer (o "pong"), que já está cheio, para aplicar filtros, normalizar ou enviar para o display/reprodução. Quando o DMA enche o buffer "ping", ele sinaliza a CPU e começa a preencher o "pong". A CPU, por sua vez, passa a processar o "ping". Isso cria um fluxo contínuo de dados sem interrupções, essencial para um processamento de áudio mais complexo sem parar a gravação.
2. Como é possível gravar áudios mais extensos, sem prejudicar a qualidade da gravação?
A principal limitação para a duração da gravação é a quantidade de memória RAM disponível no microcontrolador. A fórmula para calcular o espaço necessário é: Tamanho do Buffer = Taxa de Amostragem × Duração × Tamanho da Amostra. Com a RAM limitada do Pico, gravar por muitos segundos a uma alta qualidade rapidamente esgota a memória. Para contornar isso, as seguintes estratégias podem ser usadas:

____________________________________________________________________________________________
Uso de Memória Externa:

A solução mais eficaz é adicionar um módulo de cartão SD ao projeto, conforme sugerido no documento. O cartão SD oferece uma capacidade de armazenamento ordens de magnitude maior que a RAM interna.
Implementação: Em vez de armazenar todo o áudio em um único buffer gigante na RAM, usaríamos um ou dois buffers menores (como na técnica de double buffering). Assim que um buffer na RAM estivesse cheio com os dados do ADC, o programa principal escreveria o conteúdo desse buffer no cartão SD. Esse processo se repetiria, criando um arquivo de áudio no cartão do tamanho que quisermos. Para a reprodução, o processo inverso seria feito: leríamos blocos de dados do arquivo no cartão SD para os buffers na RAM, que seriam então enviados para o PWM.

____________________________________________________________________________________________
Compressão de Áudio em Tempo Real:

Outra abordagem é reduzir a quantidade de dados necessários para armazenar o áudio através da compressão, também citada como uma possibilidade no projeto.
ADPCM (Adaptive Differential Pulse-Code Modulation): É uma técnica de compressão com perdas muito popular em microcontroladores. Em vez de armazenar o valor completo de cada amostra (que no Pico tem 12 bits), o ADPCM armazena apenas a diferença entre a amostra atual e a anterior. Como essa diferença é geralmente um valor pequeno, ela pode ser representada com menos bits (ex: 4 bits). Isso pode reduzir o tamanho dos dados em 2 a 3 vezes, permitindo gravar um áudio 2 ou 3 vezes mais longo no mesmo espaço de RAM, com uma perda de qualidade mínima e aceitável para aplicações de voz.

____________________________________________________________________________________________
Redução da Qualidade (Quando Aceitável):

A maneira mais simples de aumentar a duração é reduzir a qualidade. Isso pode ser feito de duas formas, ambas resultando em um arquivo menor:
Diminuir a Taxa de Amostragem: O projeto sugere taxas como 8 kHz, 11 kHz, 16 kHz ou 22 kHz. Reduzir a taxa de 16 kHz para 8 kHz, por exemplo, dobra a duração da gravação que cabe na memória, mas limita as frequências de áudio que podem ser capturadas (resultando em um som mais "abafado").
Diminuir a Profundidade de Bits (Bit Depth): O ADC do Pico tem 12 bits de resolução. Poderíamos descartar os bits menos significativos e trabalhar com apenas 8 bits por amostra. Isso reduz o uso de memória em 33% (passando de 12 para 8 bits), mas diminui a faixa dinâmica do áudio (a diferença entre os sons mais baixos e mais altos), o que pode introduzir um ruído de quantização.
Combinando essas técnicas, especialmente o uso de um cartão SD com streaming de dados via DMA e double buffering, é possível gravar áudios de duração praticamente ilimitada sem comprometer a qualidade original da amostragem.
