#include "input_handler.hpp"
#include <Arduino.h>

#include "config.hpp"

InputHandler::InputHandler(const uint16_t NoPressValue, const uint16_t LeftAnalogValue,
        const uint16_t MiddleAnalogValue, const uint16_t RightAnalogValue,
        const uint16_t ShortPressThreshold, const uint16_t LongPressThreshold)
{

    this->NoPressValue = NoPressValue;
    this->LeftAnalogValue = LeftAnalogValue;
    this->MiddleAnalogValue = MiddleAnalogValue;
    this->RightAnalogValue = RightAnalogValue;

    this->ShortPressThreshold = ShortPressThreshold;
    this->LongPressThreshold = LongPressThreshold;

    pinMode(config::PinConfig::BUTTON_PANEL, INPUT);
}

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

bool InputHandler::aroundValue(const uint16_t reference_value, const uint16_t compared_value)
{
    if (std::abs(reference_value - compared_value) < AnalogReadErrorMargin)
    {
        return true;
    }
    return false;
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

    for (uint16_t i = 0; i < SamplesPerRead; i++)
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
