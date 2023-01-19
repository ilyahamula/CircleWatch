#pragma once

//#define USE_BLUETOOTH

// buttons
#define HOUR_BTN_PIN 23
#define MIN_BTN_PIN 22
#define LONG_PRESS_TIME 1000
#define SHORT_PRESS_TIME 300

// led and light
#define LIGHT_SENSOR_PIN_A 25
#define DIAL_PIN 12
#define LIGHT_PIN 13
#define LIGHT_OUTER_END_IDX 35

#define MINUTES 60
#define HOURS 12

#define LIGHT_LED_COUNT 60
#define DIAL_LED_COUNT 111
#define DEFAULT_BRIGHTNESS 100

using uint8_t = unsigned char;
using TaskHandle_t = void*;
