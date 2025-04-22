# Projeto: Contador com Interrupções e Display OLED - Raspberry Pi Pico

Este projeto implementa um contador decrescente (de 9 a 0), utilizando o Raspberry Pi Pico, controlado por botões com feedback visual via display OLED (SSD1306), utilizando a plataforma Raspberry Pi Pico (RP2040). O sistema exibe um temporizador crescente e permite o controle através de dois botões: iniciar/parar o contador (Botão A) e contar cliques (Botão B).

-----

Objetivo

Permitir que:
- Um botão inicie uma contagem regressiva de 9 a 0.
- Um segundo botão conte cliques durante a contagem.
- Todas as informações sejam exibidas em tempo real no display OLED.

Funcionalidades

- Interrupções GPIO para resposta instantânea aos botões.
- Timer para decremento automático do contador a cada 1 segundo.
- Feedback em tempo real via display SSD1306 (I2C).
- Função debounce para evitar múltiplas detecções por clique.

Esquema de Conexão

| Componente                     | Pino Pico  | Função           |
|--------------------------------|------------|------------------|
| Botão A                    --  | GPIO 5     | Inicia contagem  |
| Botão B                    --  | GPIO 6     | Conta cliques    |
| Display OLED (SDA)             | GPIO 14    | Dados I2C        |
| Display OLED (SCL)             | GPIO 15    | Clock I2C        |

> O display utiliza o endereço I2C padrão 0x3C.

---

Dependências

- Raspberry Pi Pico SDK
- Biblioteca SSD1306 (compatível com o display I2C 128x64)
- `hardware/gpio.h`, `hardware/irq.h`, `hardware/timer.h`, `hardware/i2c.h`, `pico/stdlib.h`


Como usar
Conecte o Pico ao seu computador.
Suba o código compilado para o Pico (ex: arrastando .uf2 gerado).
Pressione Botão A para iniciar a contagem.
Pressione Botão B enquanto o contador está ativo para registrar os cliques.

---

📝 Licença
Distribuído sob a licença MIT. Veja LICENSE para mais informações

---
Adriana Rocha

Projeto desenvolvido para fins educacionais e de experimentação com sistemas embarcados.
