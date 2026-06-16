#include "../include/display_handler.hpp"
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <string>

DisplayHandler::DisplayHandler() {
  this->display = Adafruit_SSD1306(m_width, m_height, &Wire, -1);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
  }
  delay(15);
  draw_on_boot();
  display.setTextSize(1);
  display.clearDisplay();
}
DisplayHandler::~DisplayHandler() {
  if (m_song_name) {
    delete m_song_name;
  }
}
void DisplayHandler::change_song_name(const char *song_path) {
  m_song_name = parse_name(song_path);

  display.setTextWrap(true);
  display.fillRect(0, 26, 128, 38, 0x0000);
  display.setCursor(19, 35);
  display.println(m_song_name);
  display.display();
}
void DisplayHandler::change_battery_voltage(float battery_voltage) {
  m_battery_voltage = battery_voltage;

  display.setTextWrap(false);

  display.fillRect(109, 0, 128, 12, 0x0000);
  display.setCursor(111, 3);
  display.println(battery_voltage);
  display.display();
}
void DisplayHandler::change_volume_level(float volume_level) {
  m_volume_level = volume_level;

  display.setTextWrap(false);

  display.fillRect(24, 1, 30, 12, 0x0000);
  display.setCursor(25, 3);
  display.println(volume_level);
  display.display();
}
void DisplayHandler::draw_pause() {
  display.setTextWrap(false);

  display.fillRect(55, 2, 30, 10, 0x0000);
  display.setCursor(55, 2);
  display.println("Psd");
  display.display();
}
void DisplayHandler::draw_play() {
  display.setTextWrap(false);
  display.fillRect(55, 2, 30, 10, 0x0000);
  display.setCursor(55, 2);
  display.println("Ply");
  display.display();
}
void DisplayHandler::dim_screen(const bool cond) { display.dim(cond); }

void DisplayHandler::draw_song_template() {
  display.clearDisplay();
  display.setTextSize(1);

  display.setTextWrap(false);
  display.setCursor(87, 3);
  display.println("Bat:");

  display.setCursor(5, 17);
  display.println("Now playing:");

  display.setCursor(2, 3);
  display.println("Vol");

  display.display();
}
void DisplayHandler::show_song_screen(const char *song_path,
                                      float battery_voltage,
                                      float volume_level) {
  dim_screen(true);
  draw_song_template();
  change_volume_level(volume_level);
  change_song_name(song_path);
  change_battery_voltage(battery_voltage);
  draw_pause();
  dim_screen(false);

}
void DisplayHandler::draw_on_boot() {
  display.clearDisplay();

  display.setTextSize(3);
  display.setCursor(40, 20);
  display.setTextColor(WHITE);
  display.println(":3");
  display.display();
}
char *DisplayHandler::parse_name(const char *song_path) {
  size_t last_slash_idx = 0;
  size_t current_idx = 0;
  while (song_path[current_idx] != '\0') {
    if (song_path[current_idx] == '/') {
      last_slash_idx = current_idx;
    }
    current_idx++;
  }

  if (last_slash_idx > strlen(song_path) - 1) {
    return nullptr;
  }

  const char *tmp_name = song_path + last_slash_idx + 1;
  char *song_name = strdup(tmp_name);
  return song_name;
}
