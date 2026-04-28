
class InputHandler {

    const int no_press_value;

    const int left_analog_value;
    const int middle_analog_value;
    const int right_analog_value;

    const int long_press_treshold;
    const int short_press_treshold;

    char debounce_timer_left;
    char debounce_timer_middle;
    char debounce_timer_right;

    char debounce_timer_treshhold;

    bool around_value(int reference_value, int compared_value);

  public:
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