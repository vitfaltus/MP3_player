#ifndef FIRMWARE_INCLUDE_INPUT_HANDLER_HPP
#define FIRMWARE_INCLUDE_INPUT_HANDLER_HPP
#include <cstdint>





class InputHandler
{

public:
    InputHandler(uint16_t NoPressValue, uint16_t LeftAnalogValue,
        uint16_t MiddleAnalogValue, uint16_t RightAnalogValue,
        uint16_t ShortPressThreshold, uint16_t LongPressThreshold);

    enum ButtonPress
    {
        None,
        LeftButtonPress,
        MiddleButtonPress,
        RightButtonPress,
        LeftButtonLongPress,
        MiddleButtonLongPress,
        RightButtonLongPress
    };

    ButtonPress checkButtons();

private:
    uint16_t NoPressValue = 0;

    uint16_t LeftAnalogValue = 0;
    uint16_t MiddleAnalogValue = 0;
    uint16_t RightAnalogValue = 0;

    uint16_t LongPressThreshold = 0;
    uint16_t ShortPressThreshold = 0;

    uint32_t LeftDebounceTimer = 0;
    uint32_t MiddleDebounceTimer = 0;
    uint32_t RightDebounceTimer = 0;

    static constexpr uint16_t SamplesPerRead = 8;
    uint16_t ButtonPanelValue = 0;

    static constexpr uint16_t AnalogReadErrorMargin = 45;

    [[nodiscard]] static bool aroundValue(uint16_t reference_value,
                                          uint16_t compared_value);

    void getButtonPanelValue();

    ButtonPress handleLeftRelease();
    ButtonPress handleMiddleRelease();
    ButtonPress handleRightRelease();

    void clearDebounceTimers();
};

#endif
