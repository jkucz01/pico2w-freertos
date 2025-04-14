#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "OLED_1in3_c.h"
#include "GUI_Paint.h"
#include "fonts.h"

#define IMAGE_WIDTH OLED_1in3_C_WIDTH
#define IMAGE_HEIGHT OLED_1in3_C_HEIGHT
#define IMAGE_SIZE ((IMAGE_WIDTH + 7) / 8 * IMAGE_HEIGHT)
static UBYTE image[IMAGE_SIZE];

void OLED_task(void *pvParameters) {
    DEV_Delay_ms(100);

    printf("OLED_test Demo\r\n");

    if (DEV_Module_Init() != 0) {
        printf("DEV_Module_Init failed.\n");
        vTaskDelete(NULL);
    }

    OLED_1in3_C_Init();
    OLED_1in3_C_Clear();

    Paint_NewImage(image, IMAGE_WIDTH, IMAGE_HEIGHT, 0, WHITE);
    Paint_Clear(BLACK);

    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    OLED_1in3_C_Display(image);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    DEV_Module_Exit();
    vTaskDelete(NULL);
}

int main() {
    stdio_init_all();
    sleep_ms(4000);

    xTaskCreate(OLED_task, "OLED Task", 512, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true) tight_loop_contents();
}
