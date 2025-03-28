#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define TIMER_PERIOD 1000

TimerHandle_t timer_handle;

void timer_callback(TimerHandle_t xTimer) {
    printf("Hello FreeRTOS world!\n");
}

int main () {
    stdio_init_all();

    timer_handle = xTimerCreate("1 second timer", TIMER_PERIOD, pdTRUE, NULL, timer_callback);
    xTimerStart(timer_handle, 0);
    vTaskStartScheduler();

    // Never reached
    while (true) {
        tight_loop_contents();
    }
}