
#ifndef SSD1306_H
#define SSD1306_H

#include "pico/stdlib.h"
#include <stdint.h>
#include <stdbool.h>

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

typedef enum {
    SSD1306_COLOR_BLACK = 0x00,
    SSD1306_COLOR_WHITE = 0x01
} SSD1306_COLOR;

bool SSD1306_Init(void);
void SSD1306_Fill(SSD1306_COLOR color);
void SSD1306_UpdateScreen(void);
void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
void SSD1306_SetCursor(uint8_t x, uint8_t y);
void SSD1306_DrawChar(char ch, SSD1306_COLOR color);
void SSD1306_DrawString(const char* str, SSD1306_COLOR color);

#endif
