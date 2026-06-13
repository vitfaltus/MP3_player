//
// Created by archer on 09.06.26.
//

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

class display {
  Adafruit_SSD1306 display;
  int width, height;
  public:
  display();

};

#ifndef FIRMWARE_DISPLAY_HPP
#define FIRMWARE_DISPLAY_HPP

#endif // FIRMWARE_DISPLAY_HPP
