#ifndef OLED_H
#define OLED_H

#include "DEV_Config.h"

#define LINE_HEIGHT 16
#define MAX_LINES (OLED_1in3_C_HEIGHT / LINE_HEIGHT)
#define IMAGE_BUFFER_SIZE ((OLED_1in3_C_WIDTH + 7) / 8 * OLED_1in3_C_HEIGHT)

int OLED_init(void);
void OLED_write(int line, const char *text);

#endif