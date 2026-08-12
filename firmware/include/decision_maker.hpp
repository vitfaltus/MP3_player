#ifndef FIRMWARE_DECISION_MAKER_HPP
#define FIRMWARE_DECISION_MAKER_HPP
#include "audio_settings.hpp"
#include "battery_manager.hpp"
#include "display_handler.hpp"
#include "file_system_manager.hpp"
#include "input_handler.hpp"
#include "playlist.hpp"

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
    Playlist* playlist;

    // menu selector
    uint8_t MenuSelectorPosition;

    // --- methods correspondent to all possible device states ---
    void songPlayingAction(InputHandler::ButtonPress buttonPress);
    void menuAction(InputHandler::ButtonPress buttonPress);
    void songSelectAction(InputHandler::ButtonPress buttonPress);
    void settingsAction(InputHandler::ButtonPress buttonPress);
    void debugAction(InputHandler::ButtonPress buttonPress);


    // menu state related methods
    void shiftMenuSelectorUp();
    void shiftMenuSelectorDown();

    public:
    DecisionMaker();
    ~DecisionMaker();

    void performedAction(InputHandler::ButtonPress buttonPress);

    DisplayHandler* getDisplayHandler() const;
};

#endif // FIRMWARE_DECISION_MAKER_HPP
