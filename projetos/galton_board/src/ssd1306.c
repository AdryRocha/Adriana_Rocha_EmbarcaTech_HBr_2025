
#include "ssd1306.h"
#include "fonts.h"
#include "hardware/i2c.h"
#include <string.h>

#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5
#define SSD1306_I2C_ADDR 0x3C

static uint8_t buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
static uint8_t cursor_x = 0, cursor_y = 0;

static void ssd1306_write_cmd(uint8_t cmd) {
    uint8_t data[2] = {0x00, cmd};
    i2c_write_blocking(I2C_PORT, SSD1306_I2C_ADDR, data, 2, false);
}

bool SSD1306_Init(void) {
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_write_cmd(0xAE);
    ssd1306_write_cmd(0x20);
    ssd1306_write_cmd(0x10);
    ssd1306_write_cmd(0xB0);
    ssd1306_write_cmd(0xC8);
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0x10);
    ssd1306_write_cmd(0x40);
    ssd1306_write_cmd(0x81);
    ssd1306_write_cmd(0xFF);
    ssd1306_write_cmd(0xA1);
    ssd1306_write_cmd(0xA6);
    ssd1306_write_cmd(0xA8);
    ssd1306_write_cmd(0x3F);
    ssd1306_write_cmd(0xA4);
    ssd1306_write_cmd(0xD3);
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0xD5);
    ssd1306_write_cmd(0xF0);
    ssd1306_write_cmd(0xD9);
    ssd1306_write_cmd(0x22);
    ssd1306_write_cmd(0xDA);
    ssd1306_write_cmd(0x12);
    ssd1306_write_cmd(0xDB);
    ssd1306_write_cmd(0x20);
    ssd1306_write_cmd(0x8D);
    ssd1306_write_cmd(0x14);
    ssd1306_write_cmd(0xAF);
    return true;
}

void SSD1306_Fill(SSD1306_COLOR color) {
    memset(buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(buffer));
}

void SSD1306_UpdateScreen(void) {
    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_write_cmd(0xB0 + page);
        ssd1306_write_cmd(0x00);
        ssd1306_write_cmd(0x10);
        uint8_t data[129];
        data[0] = 0x40;
        memcpy(&data[1], &buffer[SSD1306_WIDTH * page], SSD1306_WIDTH);
        i2c_write_blocking(I2C_PORT, SSD1306_I2C_ADDR, data, sizeof(data), false);
    }
}

void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;
    if (color == SSD1306_COLOR_WHITE)
        buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
    else
        buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
}

void SSD1306_SetCursor(uint8_t x, uint8_t y) {
    cursor_x = x;
    cursor_y = y;
}

void SSD1306_DrawChar(char ch, SSD1306_COLOR color) {
    if (ch < 32 || ch > 127) ch = '?';
    for (int i = 0; i < 5; i++) {
        uint8_t line = font_5x7[ch - 32][i];
        for (int j = 0; j < 8; j++) {
            SSD1306_DrawPixel(cursor_x + i, cursor_y + j, (line & 1) ? color : SSD1306_COLOR_BLACK);
            line >>= 1;
        }
    }
    cursor_x += 6;
}

void SSD1306_DrawString(const char* str, SSD1306_COLOR color) {
    while (*str) SSD1306_DrawChar(*str++, color);
}
