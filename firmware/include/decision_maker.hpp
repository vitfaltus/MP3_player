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

    DisplayHandler* display_handler;
    FileSystemManager* file_system_manager;
    AudioSettings* audio_settings;
    Playlist* playlist;




    void songPlayingAction(InputHandler::ButtonPress buttonPress);


    public:
    DecisionMaker();
    ~DecisionMaker();

    void performedAction(InputHandler::ButtonPress buttonPress);

    DisplayHandler* getDisplayHandler();
};

#endif // FIRMWARE_DECISION_MAKER_HPP
