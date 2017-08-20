# ESP8266 based MQTT enabled Temp/Humidity sensor
This code contais code for a temperature and humidity sensor based on the esp8266 microprocessor. 

## Hardware Architecture
In this setup I used the NodeMCU esp8266 board that facilitates the popular DHT11 sensor for measurement. The board is wifi enabled so there is no further extension shield required to access the internet.

## Running the Software
The software uses the [esp-open-rtos](https://github.com/SuperHouse/esp-open-rtos) real time operating system. You will need to use the [esp-open-sdk](https://github.com/pfalcon/esp-open-sdk/) to build and upload the software onto the chip.


