#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "btstack.h"


static btstack_packet_callback_registration_t hci_event_callback_registration;

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    UNUSED(size);
    UNUSED(channel);

    if (packet_type != HCI_EVENT_PACKET) return;
    switch(hci_event_packet_get_type(packet)){
        case BTSTACK_EVENT_STATE:
            if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) return;

            gap_set_scan_parameters(1, 0x30, 0x30);
            gap_start_scan();
            printf("Scanning started...\n");
            break;
        case GAP_EVENT_ADVERTISING_REPORT: 
            bd_addr_t server_addr;
            gap_event_advertising_report_get_address(packet, server_addr);
            uint8_t addr_type = gap_event_advertising_report_get_address_type(packet);
            uint8_t adv_size = gap_event_advertising_report_get_data_length(packet);
            const uint8_t *adv_data = gap_event_advertising_report_get_data(packet);
            
            for (int i = 0; i < adv_size;) {
                uint8_t field_length = adv_data[i];
                if (field_length == 0) break;
                uint8_t field_type = adv_data[i + 1];
            
                if (field_type == BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME || field_type == BLUETOOTH_DATA_TYPE_SHORTENED_LOCAL_NAME) {
                    char device_name[field_length];
                    memcpy(device_name, &adv_data[i + 2], field_length - 1);
                    device_name[field_length - 1] = '\0';
            
                    printf("Device name found: %s\n", device_name);
            
                    if (strcmp(device_name, "Pico-BLE 1") == 0) {
                        printf("Found target device: %s\n", bd_addr_to_str(server_addr));
                        gap_stop_scan();
                        gap_connect(server_addr, 0);
                    }
                }
                i += field_length + 1;
            }
            break;
        case HCI_EVENT_DISCONNECTION_COMPLETE:
            printf("Disconnected...\n");
            gap_start_scan();
            break;
        case HCI_EVENT_LE_META:
            if (hci_event_le_meta_get_subevent_code(packet) == HCI_SUBEVENT_LE_CONNECTION_COMPLETE) {
                uint8_t status = hci_subevent_le_connection_complete_get_status(packet);
                if (status == 0) {
                    printf("Connected to Pico-BLE 1!\n");
                } else {
                    printf("Connection failed, retrying...\n");
                    gap_start_scan();
                }
            }
            break;
        default:
            break;
    }
}

void bt_task(void *pvParameters) {
    if (cyw43_arch_init()) {
        printf("Failed to initialize Wi-Fi/Bluetooth chip.\n");
        return;
    }

    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    hci_power_control(HCI_POWER_ON);

    while (true) {
        sleep_ms(1000);
    }
}

int main() {
    stdio_init_all();
    sleep_ms(4000);

    printf("Starting BLE Client...\n");

    xTaskCreate(bt_task, "BLE Client Task", 1024, NULL, 1, NULL);

    vTaskStartScheduler();

    // Never reached
    while (true) tight_loop_contents();
}