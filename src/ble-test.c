/* 
    Adapted from https://github.com/raspberrypi/pico-examples/ 
*/

#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "btstack.h"

static btstack_packet_callback_registration_t hci_event_callback_registration;
int bt_init(void);

static uint8_t adv_data[] = {
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, 0x06,
    0x0B, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'P', 'i', 'c', 'o', '-', 'B', 'L', 'E', ' ', '1',
    0x03, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS, 0x1A, 0x18
};


static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    UNUSED(size);
    UNUSED(channel);

    bd_addr_t local_addr;

    if (packet_type != HCI_EVENT_PACKET) return;
    switch(hci_event_packet_get_type(packet)){
        case BTSTACK_EVENT_STATE:
            if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) return;
            gap_local_bd_addr(local_addr);
            printf("BTstack up and running on %s.\n", bd_addr_to_str(local_addr));
            bd_addr_t null_addr;
            memset(null_addr, 0, 6);
            gap_advertisements_set_params(800, 800, 0, 0, null_addr, 0x07, 0x00);
            sleep_ms(500);
            gap_advertisements_set_data(sizeof(adv_data), (uint8_t*) adv_data);
            gap_advertisements_enable(1);
            printf("Advertising started...\n");
            break;
        case HCI_EVENT_DISCONNECTION_COMPLETE:
            printf("Disconnected...\n");
            gap_advertisements_enable(1);
            break;
        case HCI_EVENT_LE_META:
            if (hci_event_le_meta_get_subevent_code(packet) == HCI_SUBEVENT_LE_CONNECTION_COMPLETE) {
                printf("Device connected!\n");
                gap_advertisements_enable(0);
            }
            break;
        default:
            break;
    }
}

void bt_task(void *pvParameters) {
    int res = bt_init();
    if (res) return;

    hci_power_control(HCI_POWER_ON);
    while(true) {      
        sleep_ms(1000);
    }
}

int bt_init(void) {
    if (cyw43_arch_init()) {
        printf("failed to initialise cyw43_arch\n");
        return -1;
    }
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);
    return 0;
}

int main() {
    stdio_init_all();
    sleep_ms(4000);

    printf("Program started\n");

    xTaskCreate(bt_task, "BLE Task", 1024, NULL, 1, NULL);

    vTaskStartScheduler();

    // Never reached
    while (true) tight_loop_contents();
}