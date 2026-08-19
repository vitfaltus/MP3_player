#ifndef FIRMWARE_DECISION_MAKER_HPP
#define FIRMWARE_DECISION_MAKER_HPP
#include "audio_settings.hpp"
#include "battery_manager.hpp"
#include "display_handler.hpp"
#include "file_system_manager.hpp"
#include "input_handler.hpp"
#include "song_player.hpp"
enum E_DeviceState
{
    song_playing,
    menu,
    song_select,
    settings,
    settings_volume,
    settings_timeout,
    debug
};

class DecisionMaker
{
    E_DeviceState DeviceState;

    // device subsystems
    C_DisplayHandler* DisplayHandler;
    C_FileSystemManager* FileSystemManager;
    C_AudioSettings* AudioSettings;
    C_SongPlayer* SongPlayer;

    // menu selector
    uint8_t MenuSelectorPosition;

    // song select vars
    std::array<char*, 3> DisplaySongArr;
    uint8_t SongSelectSelectorPosition;

    // settings related vars
    uint8_t SettingsSelectorPosition;


    // --- methods correspondent to all possible device states ---
    void songPlayingAction(InputHandler::ButtonPress buttonPress);
    void menuAction(InputHandler::ButtonPress buttonPress);
    void songSelectAction(InputHandler::ButtonPress buttonPress);
    void settingsAction(InputHandler::ButtonPress buttonPress);
    void settingsVolumeAction(InputHandler::ButtonPress buttonPress);
    void settingsTimeoutAction(InputHandler::ButtonPress buttonPress);
    void debugAction(InputHandler::ButtonPress buttonPress);


    // menu state related methods
    void shiftMenuSelectorUp();
    void shiftMenuSelectorDown();
    void changeToMenu();


    // song select related methods
    void fetchAndDisplaySongs();
    void shiftSongSelectorUp();
    void shiftSongSelectorDown();
    void changeToSongPlaying();

    // settings related methods
    void shiftSettingsSelectorUp();
    void shiftSettingsSelectorDown();
    //void changeToSettings();

    //void changeToSettingVolume();
    //void changeToSettingTimeout();
    


    public:
    DecisionMaker();
    ~DecisionMaker();

    void performedAction(InputHandler::ButtonPress buttonPress);

    C_DisplayHandler* getDisplayHandler() const;
};

#endif // FIRMWARE_DECISION_MAKER_HPP
