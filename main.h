#ifndef MAIN_H
#define MAIN_H
#include "FreeRTOS.h"
#include <semphr.h>

#define PUB_MSG_LEN 66
QueueHandle_t mqtt_msg_queue;
SemaphoreHandle_t wifi_alive;
#endif /* MAIN_H */

