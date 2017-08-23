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

void getTempMsg(char* buffer, int value);
void getHumidMsg(char* buffer, int value);
void getValueString(char* buffer, int value);

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
            char tempMsg[64];
            getTempMsg(tempMsg, temperature);
            if (xQueueSend(mqtt_msg_queue, (void *)tempMsg, 0) == pdFALSE) {
                printf("Publish queue overflow.\r\n");
            }
            char humidMsg[64];
            getHumidMsg(humidMsg, humidity);
            if (xQueueSend(mqtt_msg_queue, (void *)humidMsg, 0) == pdFALSE) {
                printf("Publish queue overflow.\r\n");
            }
        } else {
            printf("Failed to read data from sensor\n");
        }
    }
}

// renders the message format for a temperature value
void getTempMsg(char* buffer, int value)
{
    const char* template = "temperature,location=livingroom value=";
    char intString[5];
    getValueString(intString, value);
    strcpy(buffer,template);
    strcat(buffer,intString);
}

// renders the message format for a humidity value
void getHumidMsg(char* buffer, int value)
{
    const char* template = "humidity,location=livingroom value=";
    char intString[5];
    getValueString(intString, value);
    strcpy(buffer,template);
    strcat(buffer,intString);
}

void getValueString(char* buffer, int value)
{
    int beforeDecimal = value / 10;
    int afterDecimal = value % 10;
    snprintf(buffer,5,"%i.%i",beforeDecimal,afterDecimal);
}

