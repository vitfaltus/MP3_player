#include "decision_maker.hpp"


DecisionMaker::DecisionMaker()
{
    deviceState = song_playing;

    display_handler = new DisplayHandler();
    file_system_manager = new FileSystemManager();
    audio_settings = new AudioSettings();
    playlist = new SongPlayer("/");

    // setting current song as the one saved in the .current_song
    char* tmp = nullptr;
    if (char** tmp_ptr = &tmp; file_system_manager->getCurrentSongPath(tmp_ptr))
    {
        playlist->rollToSong(tmp);
    }

    delete[] tmp;

    display_handler->showSongScreen(playlist->getSongName(),
                                    BatteryManager::getBatteryVoltage(),
                                    audio_settings->getVolume());

    MenuSelectorPosition = 0;
}

// calls method of the current state of the machine, passes the buttonPress as a param
void DecisionMaker::performedAction(InputHandler::ButtonPress buttonPress)
{
    switch (deviceState)
    {
    case song_playing:
        songPlayingAction(buttonPress);
        break;
    case menu:
        menuAction(buttonPress);
        break;
    case song_select:
        songSelectAction(buttonPress);
        break;
    case settings:
        settingsAction(buttonPress);
        break;
    case debug:
        debugAction(buttonPress);
        break;
    default:
        break;
    }
}

DisplayHandler* DecisionMaker::getDisplayHandler() const
{
    return display_handler;
}

void DecisionMaker::songPlayingAction(InputHandler::ButtonPress buttonPress)
{
    if (display_handler->displayDimmingRoutine(buttonPress))
    {
        return;
    }

    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress: // volume down
        audio_settings->volumeDown();
        display_handler->changeVolumeLevel(audio_settings->getVolume());
        break;
    case InputHandler::LeftButtonLongPress: // previous song
        playlist->playPreviousSong(audio_settings);
        display_handler->changeSongName(playlist->getSongName());
        file_system_manager->setCurrentSongPath(playlist->getSongName());
        break;
    case InputHandler::MiddleButtonPress: // pause/play song
        if (playlist->isPaused())
        {
            playlist->play(audio_settings);
            audio_settings->restoreAudio();
            display_handler->drawPlay();
        }
        else
        {
            playlist->stop();
            audio_settings->shutAudio();
            display_handler->drawPause();
        }
        break;
    case InputHandler::MiddleButtonLongPress: // change to menu
        playlist->stop();
        audio_settings->shutAudio();

        deviceState = menu;
        MenuSelectorPosition = 0;
        display_handler->drawMenuScreen(MenuSelectorPosition);
        break;
    case InputHandler::RightButtonPress: // volume up
        audio_settings->volumeUp();
        display_handler->changeVolumeLevel(audio_settings->getVolume());
        break;
    case InputHandler::RightButtonLongPress: // next song
        playlist->playNextSong(audio_settings);
        display_handler->changeSongName(playlist->getSongName());
        file_system_manager->setCurrentSongPath(playlist->getSongName());
        break;
    default:
        break;
    }
    if (playlist->playerLoop(audio_settings))
    {
        display_handler->changeSongName(playlist->getSongName());
        file_system_manager->setCurrentSongPath(playlist->getSongName());
    }
}

void DecisionMaker::menuAction(InputHandler::ButtonPress buttonPress)
{

    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress: // cycles menu selector up
        shiftMenuSelectorUp();
        break;
    case InputHandler::LeftButtonLongPress: // nothing

        break;
    case InputHandler::MiddleButtonPress: // selects menu element on the selector
        switch (MenuSelectorPosition) {
            case 0: // song selection
                deviceState = song_select;
            break;
            case 1: // settings
                deviceState = settings;
            break;
            case 2: // debug
                deviceState = debug;
                multi_heap_info_t info;
                display_handler->drawDebugScreen(info);
            break;

            default:
            break;
        }
        break;
    case InputHandler::MiddleButtonLongPress: // back to song playing
        deviceState = song_playing;
        display_handler->showSongScreen(playlist->getSongName(),
                                   BatteryManager::getBatteryVoltage(),
                                   audio_settings->getVolume());
        //resets the dimming timer
        display_handler->displayDimmingRoutine(InputHandler::ButtonPress::MiddleButtonPress);


        break;
    case InputHandler::RightButtonPress: // cycles menu selector down
        shiftMenuSelectorDown();
        break;
    case InputHandler::RightButtonLongPress: // nothing

        break;
    default:
        break;
    }
}
void DecisionMaker::songSelectAction(InputHandler::ButtonPress buttonPress) {}

void DecisionMaker::settingsAction(InputHandler::ButtonPress buttonPress) {}

void DecisionMaker::debugAction(InputHandler::ButtonPress buttonPress)
{
    if (buttonPress != InputHandler::None) {
        deviceState = menu;
        display_handler->drawMenuScreen(MenuSelectorPosition);
    }
}

void DecisionMaker::shiftMenuSelectorUp()
{
    if (MenuSelectorPosition > 0) {
        MenuSelectorPosition--;
    }
    display_handler->drawMenuScreen(MenuSelectorPosition);

}

void DecisionMaker::shiftMenuSelectorDown()
{
    if (MenuSelectorPosition < 2) {
        MenuSelectorPosition++;
    }
    display_handler->drawMenuScreen(MenuSelectorPosition);
}


