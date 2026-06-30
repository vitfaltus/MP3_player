#ifndef FIRMWARE_INCLUDE_BATTERY_MANAGER_HPP
#define FIRMWARE_INCLUDE_BATTERY_MANAGER_HPP


class BatteryManager
{
    const float MinBatteryThreshold = 3.4;

public:
    BatteryManager();
    static float getBatteryVoltage();
};

#endif
