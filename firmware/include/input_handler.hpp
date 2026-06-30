#ifndef FIRMWARE_INCLUDE_INPUT_HANDLER_HPP
#define FIRMWARE_INCLUDE_INPUT_HANDLER_HPP

class InputHandler
{

public:
    InputHandler();

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
    const int NoPressValue = 940;

    const int LeftAnalogValue = 1790;
    const int MiddleAnalogValue = 1200;
    const int RightAnalogValue = 1050;

    const int LongPressThreshold = 600;
    const int ShortPressThreshold = 100;

    int LeftDebounceTimer = 0;
    int MiddleDebounceTimer = 0;
    int RightDebounceTimer = 0;

    const int SamplesPerRead = 8;
    int ButtonPanelValue = 0;

    const int AnalogReadErrorMargin = 45;

    [[nodiscard]] bool aroundValue(int reference_value,
                                    int compared_value) const;

    void getButtonPanelValue();

    ButtonPress handleLeftRelease();
    ButtonPress handleMiddleRelease();
    ButtonPress handleRightRelease();

    void clearDebounceTimers();
};

#endif
