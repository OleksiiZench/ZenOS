#pragma once

#include <stdio.h>

#define ESP_LOGI(tag, format, ...) printf("[%s] INFO: " format "\n", tag, ##__VA_ARGS__)
#define ESP_LOGE(tag, format, ...) printf("[%s] ERROR: " format "\n", tag, ##__VA_ARGS__)
#define ESP_LOGW(tag, format, ...) printf("[%s] WARN: " format "\n", tag, ##__VA_ARGS__)
#define ESP_LOGD(tag, format, ...) printf("[%s] DEBUG: " format "\n", tag, ##__VA_ARGS__)

#define ESP_ERROR_CHECK(x) (void)(x)
