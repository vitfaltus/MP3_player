#include "battery_manager.hpp"

#include "pin_config.hpp"

#include <Arduino.h>

#include "config.hpp"

BatteryManager::BatteryManager() { pinMode(config::PinConfig::BATTERY_READ, INPUT); }
float BatteryManager::getBatteryVoltage()
{

    uint32_t Vbatt = 0;
    for (int i = 0; i < 16; i++)
    {
        Vbatt = Vbatt +
            analogReadMilliVolts(
                config::PinConfig::BATTERY_READ); // ADC with correction
    }
    const float Vbattf = 2 * Vbatt / 16 / 1000.0; // attenuation ratio 1/2, mV --> V
    return Vbattf;
}
