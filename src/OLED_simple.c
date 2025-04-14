#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "OLED.h"

void OLED_task(void *pvParameters) {

    int res = OLED_init();
    if (res) {
        printf("Failed to initialize OLED\n");
        while (true) tight_loop_contents();
    }
    OLED_write(1, "This is line 1");
    OLED_write(2, "And line 2");
    OLED_write(3, "Finally line 3");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    stdio_init_all();
    sleep_ms(4000);

    xTaskCreate(OLED_task, "OLED Task", 512, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true) tight_loop_contents();
}
