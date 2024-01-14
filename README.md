# CircleWatch

## Description
This repository contains the source code for multifunctional wall clock designed from scratch using [Seed xiao ESP32C3](https://www.seeedstudio.com/Seeed-XIAO-ESP32C3-p-5431.html) board as the core. 
Concept of design was to use the minimalistic thin dial with rgb led highlighting for time indication. Besides time the watch can be used as a smart lamp by using thick packed led strip. 
Circle watch also have various settings for customization like changing time mode, color scheme for indication, changing deep-sleep time for effective energy consumption etc.
The watch is controlled via Bluetooth Low Energy.

## Instalation
To build/load this project for ESP32 platform we need just to instal VS Code and [PlatformIO](https://platformio.org/) extension and clone the repo.

## BLE commands

### Set time
Command for setup the actual time for watch. Command with parameter for time in format hh:mm (exm. 22:05)
```bash
/set_time <hh::mm>
```
### Set hour color
Command for setting the color of hour light. Command with parameter for color in format r,g,b (exm. 255, 34, 123). Each value in range 0..255
```bash
/set_hour_color <r,g,b>
```
