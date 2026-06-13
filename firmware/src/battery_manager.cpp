#include "battery_manager.hpp"

#include "pin_config.hpp"

#include <Arduino.h>


BatteryManager::BatteryManager() {
  pinMode(PinConfig::BATTERY_READ, INPUT);
}
float BatteryManager::get_battery_voltage() {

  uint32_t Vbatt = 0;
  for(int i = 0; i < 16; i++) {
    Vbatt = Vbatt + analogReadMilliVolts(
                        PinConfig::BATTERY_READ); // ADC with correction
  }
  float Vbattf = 2 * Vbatt / 16 / 1000.0;     // attenuation ratio 1/2, mV --> V
  return Vbattf;
}
