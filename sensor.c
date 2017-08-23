#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include <dht/dht.h>
#include "esp8266.h"

#include "sensor.h"
#include "main.h"

char* getTempMsg(int value);
char* getHumidMsg(int value);

uint8_t const dht_gpio = 4; // data wire for the dht11
const dht_sensor_type_t sensor_type = DHT_TYPE_DHT11;

void dhtMeasurementTask(void *pvParameters)
{

    TickType_t xLastWakeTime = xTaskGetTickCount();
    int16_t temperature = 0;
    int16_t humidity = 0;

    // DHT sensors that come mounted on a PCB generally have
    // pull-up resistors on the data pin.  It is recommended
    // to provide an external pull-up resistor otherwise...
    gpio_set_pullup(dht_gpio, false, false);

    while(1) {
        if (dht_read_data(sensor_type, dht_gpio, &humidity, &temperature)) {

            vTaskDelayUntil(&xLastWakeTime, 20000 / portTICK_PERIOD_MS);
            char* tempMsg = getTempMsg(temperature);
            if (xQueueSend(mqtt_msg_queue, (void *)tempMsg, 0) == pdFALSE) {
                printf("Publish queue overflow.\r\n");
            }
            char* humidMsg = getHumidMsg(humidity);
            if (xQueueSend(mqtt_msg_queue, (void *)humidMsg, 0) == pdFALSE) {
                printf("Publish queue overflow.\r\n");
            }
        } else {
            printf("Failed to read data from sensor\n");
        }
    }
}

// renders the message format for a temperature value
char* getTempMsg(int value)
{
    char msg[PUB_MSG_LEN];
    snprintf(msg, PUB_MSG_LEN, 
        "weather,location=livingroom temperature=%d\n",
        value / 10);
    return msg;
}

// renders the message format for a humidity value
char* getHumidMsg(int value)
{
    char msg[PUB_MSG_LEN];
    snprintf(msg, PUB_MSG_LEN, 
        "weather,location=livingroom humidity=%d\n",
        value / 10);
    return msg;
}
