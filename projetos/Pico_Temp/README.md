# Leitor de Temperatura Interna com Display OLED - Raspberry Pi Pico

Este projeto utiliza o sensor de temperatura interno do chip RP2040 (Raspberry Pi Pico) para medir a temperatura e exibi-la em tempo real no terminal USB e em um display OLED SSD1306 via I2C.

---

Descrição

- O RP2040 possui um sensor de temperatura embutido que pode ser lido via canal 4 do ADC.
- Os dados brutos são convertidos em graus Celsius com base em valores de referência do datasheet.
- A leitura é atualizada a cada 1 segundo.
- A interface com o usuário é feita por meio de um display OLED e terminal serial (USB).


Esquema de Conexões

| Componente      | Pino Pico  | Função I2C    |
|-----------------|------------|---------------|
| OLED - SDA      | GPIO 14    | Dados (SDA)   |
| OLED - SCL      | GPIO 15    | Clock (SCL)   |
| Alimentação     | 3V3 e GND  | Energia       |




---

Adriana Rocha

Projeto desenvolvido para fins educacionais e de experimentação com sistemas embarcados.




