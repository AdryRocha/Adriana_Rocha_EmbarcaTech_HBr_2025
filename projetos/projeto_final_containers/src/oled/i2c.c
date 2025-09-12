#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../../include/config.h"

#include "i2c.h"

// Inicializa o I2C utiliza pelo display OLED
void i2c_bitdoglab_init(){
    gpio_pull_up(     I2C_MAIN_GPIO_SCL);
    gpio_pull_up(     I2C_MAIN_GPIO_SDA);
    gpio_set_function(I2C_MAIN_GPIO_SCL, GPIO_FUNC_I2C);
    gpio_set_function(I2C_MAIN_GPIO_SDA, GPIO_FUNC_I2C);
    i2c_init(         I2C_MAIN_BUS,      I2C_MAIN_BAUDRATE);
}