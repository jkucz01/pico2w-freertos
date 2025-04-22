#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "pico/stdlib.h"

#define BUTTON_A 15
#define BUTTON_B 17

void init_buttons() {
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

}

void button_task(void *pvParameters) {
    init_buttons();

    while (1) {
        if (!gpio_get(BUTTON_A)) {
            printf("Button A pressed\n");
        }
        if (!gpio_get(BUTTON_B)) {
            printf("Button B pressed\n");
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main() {
    stdio_init_all();
    sleep_ms(4000);

    xTaskCreate(button_task, "button_task", 512, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true) tight_loop_contents();
}
