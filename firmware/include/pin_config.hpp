#pragma once
#include <Arduino.h>

namespace PinConfig {

constexpr int SD_CS = D7;
constexpr int SD_SCK = D8;
constexpr int SD_MISO = D10;
constexpr int SD_MOSI = D9;

constexpr int I2S_BCLK = D2;
constexpr int I2S_LRC = D6;
constexpr int I2S_DOUT = D3;

constexpr int BATTERY_READ = D0;

constexpr int BUTTON_PANEL = D1;
} // namespace PinConfig
