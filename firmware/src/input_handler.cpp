#include "input_handler.hpp"
#include <Arduino.h>
#include "pin_config.hpp"

bool InputHandler::aroundValue(const int reference_value,const int compared_value) const
{
    if (std::abs(reference_value - compared_value) < AnalogReadErrorMargin)
    {
        return true;
    }
    return false;
}

InputHandler::InputHandler() { pinMode(PinConfig::BUTTON_PANEL, INPUT); }

InputHandler::ButtonPress InputHandler::checkButtons()
{

    getButtonPanelValue();

    // Serial.println(button_panel_value);

    if (aroundValue(NoPressValue, ButtonPanelValue))
    { // button possibly released
        if (LeftDebounceTimer > ShortPressThreshold)
        {
            return handleLeftRelease();
        }
        if (MiddleDebounceTimer > ShortPressThreshold)
        {
            return handleMiddleRelease();
        }
        if (RightDebounceTimer > ShortPressThreshold)
        {
            return handleRightRelease();
        }
        clearDebounceTimers();
    }
    if (aroundValue(LeftAnalogValue, ButtonPanelValue))
    {

        LeftDebounceTimer++;
    }
    else if (aroundValue(MiddleAnalogValue, ButtonPanelValue))
    {

        MiddleDebounceTimer++;
    }
    else if (aroundValue(RightAnalogValue, ButtonPanelValue))
    {
        RightDebounceTimer++;
    }

    return None;
}

InputHandler::ButtonPress InputHandler::handleLeftRelease()
{

    if (LeftDebounceTimer > LongPressThreshold)
    {
        clearDebounceTimers();
        return LeftButtonLongPress;
    }

    clearDebounceTimers();
    return LeftButtonPress;
}

InputHandler::ButtonPress InputHandler::handleMiddleRelease()
{
    if (MiddleDebounceTimer > LongPressThreshold)
    {
        clearDebounceTimers();
        return MiddleButtonLongPress;
    }
    clearDebounceTimers();
    return MiddleButtonPress;
}

InputHandler::ButtonPress InputHandler::handleRightRelease()
{
    if (RightDebounceTimer > LongPressThreshold)
    {
        clearDebounceTimers();
        return RightButtonLongPress;
    }
    clearDebounceTimers();
    return RightButtonPress;
}

void InputHandler::getButtonPanelValue()
{
    ButtonPanelValue = 0;

    for (int i = 0; i < SamplesPerRead; i++)
    {
        ButtonPanelValue += analogRead(PinConfig::BUTTON_PANEL);
    }
    ButtonPanelValue /= SamplesPerRead;
}

void InputHandler::clearDebounceTimers()
{
    LeftDebounceTimer = 0;
    MiddleDebounceTimer = 0;
    RightDebounceTimer = 0;
}
