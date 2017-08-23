#include <stdio.h>
#include <stdlib.h>
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "esp8266.h"

#include "sensor.h"
#include "units.h"
#include "main.h"
#include "mqtt.h"


// entry point for rtos
void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());

    vSemaphoreCreateBinary(wifi_alive);
    mqtt_msg_queue = xQueueCreate(3, PUB_MSG_LEN);
    xTaskCreate(dhtMeasurementTask, "dhtMeasurementTask", 256, NULL, 2, NULL);
    xTaskCreate(wifi_task, "wifi_task",  256, NULL, 2, NULL);
    xTaskCreate(mqtt_task, "mqtt_task", 1024, NULL, 4, NULL);
}
