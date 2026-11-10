#ifndef FIRMWARE_CONFIG_HPP
#define FIRMWARE_CONFIG_HPP
#include <cstdint>


namespace config
{
    namespace ButtonInputVals
    {
        constexpr uint16_t NoPressValue = 975;

        constexpr uint16_t LeftAnalogValue = 1085;
        constexpr uint16_t MiddleAnalogValue = 1245;
        constexpr uint16_t RightAnalogValue = 1845;

        constexpr uint16_t LongPressThreshold = 600;
        constexpr uint16_t ShortPressThreshold = 100;
    }

    namespace PinConfig
    {

        constexpr int SD_CS = D7;
        constexpr int SD_SCK = D8;
        constexpr int SD_MISO = D9;
        constexpr int SD_MOSI = D10;

        constexpr int I2S_BCLK = D2;
        constexpr int I2S_LRC = D6;
        constexpr int I2S_DOUT = D3;

        constexpr int BATTERY_READ = D0;

        constexpr int BUTTON_PANEL = D1;
    }
    namespace FileSysPaths
    {
        constexpr char CurrentSongFile[] = "/.current_song_path";
        constexpr char DefaultVolumeFile[] = "/.default_volume_path";
        constexpr char TimeOutFile[] = "/.timeout_time";


    }

}



#endif //FIRMWARE_CONFIG_HPP
