
#include "AudioOutputI2S.h"


class Audio_settings {
  AudioOutputI2S* amp;
  double volume_level = 0.3;
  const double VOLUME_DIFF = 0.05;
  public:
  Audio_settings();
  ~Audio_settings();

  double volume_up();

  void set_volume(double level);

  double volume_down();

  double get_volume();

};
