#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "pico/stdlib.h"
#include "queue.h"

#define BUTTON_A 15
#define BUTTON_B 17
#define DEBOUNCE_MS 200

QueueHandle_t button_queue;

typedef enum {
    BUTTON_EVENT_A,
    BUTTON_EVENT_B
} button_event_t;

absolute_time_t last_press_a = {0};
absolute_time_t last_press_b = {0};

void gpio_irq_handler(uint gpio, uint32_t events) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    button_event_t event;

    if (gpio == BUTTON_A && (events & GPIO_IRQ_EDGE_FALL) ) {
        event = BUTTON_EVENT_A;
        xQueueSendFromISR(button_queue, &event, &xHigherPriorityTaskWoken);
    }
    if (gpio == BUTTON_B && (events & GPIO_IRQ_EDGE_FALL) ) {
        event = BUTTON_EVENT_B;
        xQueueSendFromISR(button_queue, &event, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void init_buttons() {
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    button_queue = xQueueCreate(10, sizeof(button_event_t));

    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, (gpio_irq_callback_t) &gpio_irq_handler);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);
}

void button_task(void *pvParameters) {
    
    init_buttons();

    button_event_t event;
    while (1){
        if (xQueueReceive(button_queue, &event, portMAX_DELAY) == pdPASS) {
            absolute_time_t now = get_absolute_time();

            if (event == BUTTON_EVENT_A) {
                if (absolute_time_diff_us(last_press_a, now) > DEBOUNCE_MS * 1000) {
                    printf("button A pressed\n");
                    last_press_a = now;
                }
            } else if (event == BUTTON_EVENT_B) {
                if (absolute_time_diff_us(last_press_b, now) > DEBOUNCE_MS * 1000) {
                    printf("button B pressed\n");
                    last_press_b = now;
                }
            }
        }
    }
    
}

int main() {
    stdio_init_all();
    sleep_ms(4000);

    xTaskCreate(button_task, "button_task", 512, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true) tight_loop_contents();
}
