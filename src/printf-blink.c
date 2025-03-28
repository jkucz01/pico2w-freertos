#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define BLINK_DELAY_MS 500
#define TIMER_PERIOD 1000

TimerHandle_t timer_handle;

void timer_callback(TimerHandle_t xTimer) {
    printf("Hello FreeRTOS world!\n");
}

void blink_task(void *pvParameters) {
    if (cyw43_arch_init()) {
        printf("Failed to initialize CYW43\n");
        while (true) tight_loop_contents();
    }

    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS));

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS));
    }
}

int main() {
    stdio_init_all();

    timer_handle = xTimerCreate("1 second timer", TIMER_PERIOD, pdTRUE, NULL, timer_callback);
    xTimerStart(timer_handle, 0);

    xTaskCreate(blink_task, "LED Blink Task", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    // Never reached
    while (true) tight_loop_contents();
}