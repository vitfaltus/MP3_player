#include <Arduino.h>
#include "audio_settings.hpp"
#include "battery_manager.hpp"
#include "decision_maker.hpp"
#include "display_handler.hpp"
#include "file_system_manager.hpp"
#include "input_handler.hpp"
#include "pin_config.hpp"
#include "song_player.hpp"


InputHandler* input_handler;
BatteryManager* battery_manager;


DecisionMaker* decision_maker;

unsigned long last_millis_battery = 0;
int read_interval_millis_battery = 5000; // every 5 s


void batteryRoutine()
{
    if (const unsigned long now_millis = millis();
        now_millis - last_millis_battery > read_interval_millis_battery)
    {
        decision_maker->getDisplayHandler()->changeBatteryVoltage(
            BatteryManager::getBatteryVoltage());
        last_millis_battery = now_millis;
    }
}





void setup()
{
    Serial.begin(9600);

    decision_maker = new DecisionMaker();

    battery_manager = new BatteryManager();

    input_handler = new InputHandler();

}


void loop()
{
    const InputHandler::ButtonPress ButtonInput = input_handler->checkButtons();

    decision_maker->performedAction(ButtonInput);
    
    batteryRoutine();
}

