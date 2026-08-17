#ifndef FIRMWARE_DECISION_MAKER_HPP
#define FIRMWARE_DECISION_MAKER_HPP
#include "audio_settings.hpp"
#include "battery_manager.hpp"
#include "display_handler.hpp"
#include "file_system_manager.hpp"
#include "input_handler.hpp"
#include "SongPlayer.hpp"

enum DeviceState
{
    song_playing,
    menu,
    song_select,
    settings,
    debug
};

class DecisionMaker
{
    DeviceState deviceState;

    // device subsystems TODO refactor with PascalCase
    DisplayHandler* display_handler;
    FileSystemManager* file_system_manager;
    AudioSettings* audio_settings;
    SongPlayer* song_player;

    // menu selector
    uint8_t MenuSelectorPosition;

    // song select variables
    std::array<char*, 3> DisplaySongArr;
    uint8_t SongSelectSelectorPosition;

    // --- methods correspondent to all possible device states ---
    void songPlayingAction(InputHandler::ButtonPress buttonPress);
    void menuAction(InputHandler::ButtonPress buttonPress);
    void songSelectAction(InputHandler::ButtonPress buttonPress);
    void settingsAction(InputHandler::ButtonPress buttonPress);
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


    public:
    DecisionMaker();
    ~DecisionMaker();

    void performedAction(InputHandler::ButtonPress buttonPress);

    DisplayHandler* getDisplayHandler() const;
};

#endif // FIRMWARE_DECISION_MAKER_HPP
