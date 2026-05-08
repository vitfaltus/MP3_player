
#include "AudioOutputI2S.h"


class AudioSettings {
  AudioOutputI2S* amp;
  double m_real_volume_level = 0.3;
  double m_virtual_volume_level;

  const double VOLUME_DIFF = 0.05;
  public:
  AudioSettings();
  ~AudioSettings();

  double volume_up();

  void set_volume(double level);

  double volume_down();

  void shut_audio();

  void restore_audio();

  double get_volume();

};
