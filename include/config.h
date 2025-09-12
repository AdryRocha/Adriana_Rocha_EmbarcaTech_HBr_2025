/**
 * @file    config.h
 * @author  Adriana - Elias - Vagner
 * @brief   Configurações do hardware e definição de mensagens
 * @version 0.1
 * @date    25/08/2025
 */
#ifndef CONFIG_H
#define CONFIG_H


// #define DEBUG_BITDOG_LAB

#define BUTTON_A_GPIO                  5
#define BUTTON_B_GPIO                  6
#define BUTTON_J_GPIO                 22

#define BUTTON_DEBONCE_MS             50

// I2C_MAIN used by OLED
#define I2C_MAIN_GPIO_SCL             15
#define I2C_MAIN_GPIO_SDA             14
#define I2C_MAIN_BAUDRATE         400000
#define I2C_MAIN_BUS                i2c1

#define LED_R_GPIO                    13
#define LED_G_GPIO                    11
#define LED_B_GPIO                    12

#define QUEUE_LENGHT                 256
#define QUEUE_MSG_LENGHT              17    // 16 + 1

#define MSG_BUTTON_A_PRESSED    "BA PRESSED"
#define MSG_BUTTON_A_LED_ON     "BA LED ON"
#define MSG_BUTTON_A_RELEASE    "BA RELEASE"

#define MSG_BUTTON_B_PRESSED    "BB PRESSED"
#define MSG_BUTTON_B_LED_ON     "BB LED ON"
#define MSG_BUTTON_B_RELEASE    "BB RELEASE"

#define MSG_BUTTON_SEMAPHORO    "      SEMAPHORO"

// Mensagens de controle
#define MSG_ACCEL_START         "ACCEL_START"
#define MSG_ACCEL_STOP          "ACCEL_STOP"

#endif // CONFIG_H