#pragma once

#include <pins_arduino.h>

//#define DEBUG

#define SERVICE_UUID "8a1b5b74-15a8-44f2-9175-01fd0c08702d" // ID service
#define ELEM_UUID "42e2c66a-bff1-409e-9b99-1c7c5ad05970" // ID characteristic
#define DEVICE_MANUFACTURER "CW_ESP32"
#define DEVICE_NAME "CircleWatch"

//commands
#define SET_TIME "/set_time"
#define RESTART "/restart"
#define ADD_HOUR "/add_hour"
#define SUB_HOUR "/sub_hour"
#define SET_HOUR_COLOR "/set_hour_color"
#define SET_MIN_COLOR "/set_min_color"
#define LIGHT_OFF "/light_off"
#define LIGHT_RAINBOW "/light_rainbow_mode"
#define LIGHT_WHITE_COLD "/light_white_cold_mode"
#define LIGHT_WHITE_WARM "/light_white_warm_mode"
#define LIGHT_CUSTOM "/light_custom_mode"
#define LIGHT_BRIGHTNESS "/light_brightness"
#define LIGHT_SMOOTHE "/light_smoothe_mode" // deprecated
#define DIAL_DIGIT_HOUR "/dial_digit_hour"
#define DIAL_SEGMENT_HOUR "/dial_segment_hour"
#define DIAL_DEFAULT_COLORS "/dial_default_colors"
#define SET_DEEP_SLEEP_TIME "/set_deep_sleep_time"
#define DIAL_BRIGHTNESS "/dial_brightness"

#define CMD_PARAM_SEPARATOR ' '
#define COLOR_VALUE_SEPARATOR ','
#define TIME_SEPARATOR ':'

// buttons
//#define USE_BUTTONS
#ifdef USE_BUTTONS
    #define HOUR_BTN_PIN D6
    #define MIN_BTN_PIN D7
    #define LONG_PRESS_TIME 1000
    #define SHORT_PRESS_TIME 300
#endif

// led and light
#define LIGHT_SENSOR_PIN A0
#define DIAL_PIN D9
#define LIGHT_PIN D10
#define LIGHT_OUTER_END_IDX 35
#define NUM_SEG_DIGITS 5

#define MINUTES 60
#define HOURS 12

#define LIGHT_LED_COUNT 309
#define DIAL_LED_COUNT 111

#define DEFAULT_BRIGHTNESS 50
#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 180
#define LOW_SENSOR_VALUE 100
#define TOP_SENSOR_VALUE 3500

#define NOT_DEFINED_TIME -1
#define ADJUST_TIME_MACRO(obj)  obj.adjust(DateTime(F(__DATE__), F(__TIME__)))

// colors
#define HOUR_COLOR {255, 255, 255}
#define MIN_COLOR {0, 0, 255}

// deep sleep
#define WAKEUP_PIN D1
#define WORKING_TIME 100000 // 1 min
#define MOSFET_DIAL_PIN D7
#define MOSFET_LIGHT_PIN D8

#define BLE_SWITCH D6

using uint8_t = unsigned char;
using TaskHandle_t = void*;
