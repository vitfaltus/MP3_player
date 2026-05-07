
#include "AudioOutputI2S.h"


class AudioSettings {
  AudioOutputI2S* amp;
  double volume_level = 0.3;
  const double VOLUME_DIFF = 0.05;
  public:
  AudioSettings();
  ~AudioSettings();

  double volume_up();

  void set_volume(double level);

  double volume_down();

  double get_volume();

};
