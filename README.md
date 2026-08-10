# MP3 player
This repository is dedicated to my open source MP3 player project. The main goal of this project is to learn more about embedded software engineering and a little bit of circuit design. Apart from writing the firmware and designing the main board, I have also designed a 3D model of the device that is suitable for 3D printing.

## Hardware
### Main components
I have chosen an **ESP32C3 xiao** module for its small form factor and reasonable hardware specs. The Wi-Fi/BLE support is also welcomed for extra interaction with the player. For the actual generation of the analog signal, I have chosen **MAX98357A amplifier** module. 

### Input
The device has three buttons that each recognize either a short or a long press. On the right side, there is a sliding on/off switch, that disconnects the battery and effectively turns the device on or off.

### Output
The audio is output through a 3.5 mm jack on the back side of the device. For visual feedback, there is an OLED screen above the buttons with resolution of 128x64 px.

### Ports
The device has a USB-C connector at the bottom side to allow easy recharging of the internal battery and easy serial communication with a connected computer. Other than that, it has a 3.5 mm jack on the back and an SD card reader jetting out of its side.

## Software
The firmware is written in C++ 23 and I'm using Platformio as my development platform with Arduino framework. 

### SD card structure
As of right now, the player loads all MP3 files located in the root directory of the SD card. Directories and other files are skipped. This simple approach will be reworked in the near future to allow playlist creation and song sorting.



