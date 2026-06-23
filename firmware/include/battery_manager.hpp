#ifndef FIRMWARE_INCLUDE_BATTERY_MANAGER_HPP
#define FIRMWARE_INCLUDE_BATTERY_MANAGER_HPP


class BatteryManager {
  const float min_battery_threshold = 3.4;

public:
  BatteryManager();
  static float get_battery_voltage();
};

#endif