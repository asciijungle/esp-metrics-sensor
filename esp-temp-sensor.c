#include <stdio.h>
#include <stdlib.h>
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include <dht/dht.h>
#include "esp8266.h"

uint8_t const dht_gpio = 4; // data wire for the dht11
const dht_sensor_type_t sensor_type = DHT_TYPE_DHT11;

void dhtMeasurementTask(void *pvParameters)
{
    int16_t temperature = 0;
    int16_t humidity = 0;

    // DHT sensors that come mounted on a PCB generally have
    // pull-up resistors on the data pin.  It is recommended
    // to provide an external pull-up resistor otherwise...
    gpio_set_pullup(dht_gpio, false, false);

    while(1) {
        if (dht_read_data(sensor_type, dht_gpio, &humidity, &temperature)) {
            printf("Temp: %d°C Humidity: %d%%\n", 
                    temperature / 10, 
                    humidity / 10);
        } else {
            printf("Failed to read data from sensor\n");
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void user_init(void)
{
    uart_set_baud(0, 115200);
    xTaskCreate(dhtMeasurementTask, "dhtMeasurementTask", 256, NULL, 2, NULL);
}

