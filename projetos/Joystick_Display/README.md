# Joystick com Display OLED - Raspberry Pi Pico

Este projeto implementa um sistema para monitoramento em tempo real dos eixos X, Y e botão de um joystick analógico, utilizando o Raspberry Pi Pico e um display OLED SSD1306 via I2C. 

---
Descrição

O programa feito em linguagem C, lê continuamente os valores dos eixos X e Y do joystick através do conversor analógico-digital (ADC) e detecta o estado do botão (pressionado ou solto). Os dados são:

- exibidos no terminal (via USB);
- mostrados em tempo real no display OLED I2C.

---

Funcionalidades

- 📊 Leitura dos eixos analógicos do joystick (X e Y).
- 🔘 Leitura do botão digital (pull-up).
- 📟 Visualização em display OLED SSD1306 (I2C).
- 📤 Impressão dos dados via `printf` para terminal serial.

---

Esquema de Conexões

| Componente    | Pino Pico  | Função                      |
|---------------|------------|-----------------------------|
| Joystick X    | GPIO 26    | ADC0 (VRX)                  |
| Joystick Y    | GPIO 27    | ADC1 (VRY)                  |
| Botão (SW)    | GPIO 22    | Entrada digital com pull-up |
| OLED SDA      | GPIO 14    | I2C SDA                     |
| OLED SCL      | GPIO 15    | I2C SCL                     |

📜 Licença
Distribuído sob a licença MIT. Consulte LICENSE para mais informações.

---

Adriana Rocha

Projeto desenvolvido para fins educacionais e de experimentação com sistemas embarcados.
