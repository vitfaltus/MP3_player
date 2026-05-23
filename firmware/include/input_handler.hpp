
class InputHandler {

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

  private:
    const int no_press_value = 1070;

    const int left_analog_value = 2050;
    const int middle_analog_value = 1370;
    const int right_analog_value = 1190;

    const int long_press_treshold = 400;
    const int short_press_treshold = 50;

    int debounce_timer_left = 0;
    int debounce_timer_middle = 0;
    int debounce_timer_right = 0;

    const int debounce_timer_treshhold = 100;

    const int div_cons = 8;
    int button_panel_value = 0;

    bool around_value(int reference_value, int compared_value);

    InputHandler::button_press handle_left_release();
    InputHandler::button_press handle_middle_release();
    InputHandler::button_press handle_right_release();

    void clear_debounces();
};