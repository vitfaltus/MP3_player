#pragma once

class BatteryManager {
  const float min_battery_threshold = 3.4;

public:
  BatteryManager();
  static float get_battery_voltage();
};