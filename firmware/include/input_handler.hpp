
class InputHandler {

    const int no_press_value = 940;

    const int left_analog_value = 1060;
    const int middle_analog_value = 1200;
    const int right_analog_value = 1800;

    const int long_press_treshold = 10000;
    const int short_press_treshold = 50;

    char debounce_timer_left = 0;
    char debounce_timer_middle = 0;
    char debounce_timer_right = 0;

    char debounce_timer_treshhold = 10;

    bool around_value(int reference_value, int compared_value);

  public:
    InputHandler();

    enum button_press {
        none,
        left_button_press,
        middle_button_press,
        right_button_press,
        left_button_long_press,
        middle_button_long_press,
        right_button_long_press
    };

    button_press button_check();
};