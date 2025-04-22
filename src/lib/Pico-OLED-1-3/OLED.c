#include "OLED.h"
#include "OLED_1in3_c.h"
#include "GUI_Paint.h"

static UBYTE image_buffer[IMAGE_BUFFER_SIZE];

int OLED_init(void) {
    if (DEV_Module_Init() != 0) {
        return 1;
    }

    OLED_1in3_C_Init();
    OLED_1in3_C_Clear();

    Paint_NewImage(image_buffer, OLED_1in3_C_WIDTH, OLED_1in3_C_HEIGHT, 0, WHITE);
    Paint_Clear(BLACK);
    OLED_1in3_C_Display(image_buffer);
    return 0;
}

void OLED_write(int line, const char *text) {
    if (line < 1 || line > MAX_LINES) return;

    int y_offset = (line - 1) * LINE_HEIGHT;
    Paint_DrawString_EN(0, y_offset, text, &Font12, WHITE, BLACK);
    OLED_1in3_C_Display(image_buffer);
}