
#define ERROR_MARGIN_ANALOG_READ 45


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


    void buttons_calibration();
  private:
    const int no_press_value = 940;

    const int left_analog_value = 1790;
    const int middle_analog_value = 1200;
    const int right_analog_value = 1050;

    const int long_press_treshold = 600;
    const int short_press_treshold = 100;

    int debounce_timer_left = 0;
    int debounce_timer_middle = 0;
    int debounce_timer_right = 0;


    const int div_cons = 8;
    int button_panel_value = 0;

    static bool around_value(int reference_value, int compared_value);

    void get_button_panel_value();


    button_press handle_left_release();
    button_press handle_middle_release();
    button_press handle_right_release();

    void clear_debounces();
};