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
USB-C, jack, sd card reader

## Software
The firmware is written in C++ 23 and I'm using platformio as my development platform with arduino framework. 

### SD card structure
The SD card should be structured as follows:
/
├── songs/
│   ├── all/
│   │   ├── Interpret1-SongName1.mp3
│   │   ├── Interpret1-SongName2.mp3
│	│	└── Interpret2-SongName1.mp3
├── system/
|	├── playlist.json
│   └── settings.json



