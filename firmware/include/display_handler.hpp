//
// Created by archer on 09.06.26.
//

#include <Adafruit_SSD1306.h>



class DisplayHandler {
  Adafruit_SSD1306 display;
  int m_width = 128, m_height = 64;
  float m_battery_voltage = 0;
  char * m_song_name = nullptr;
  char * m_artist_name = nullptr;
  void draw_on_boot();
  char* parse_name(const char * song_path);

  public:
  DisplayHandler();
    ~DisplayHandler();
  void change_song_name(const char *song_path);
  void change_battery_voltage(float battery_voltage);
  void update_screen(const char * song_name, float battery_voltage);

};

