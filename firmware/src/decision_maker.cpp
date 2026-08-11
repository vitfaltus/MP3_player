#include "decision_maker.hpp"


DecisionMaker::DecisionMaker()
{
    deviceState = song_playing;

    display_handler = new DisplayHandler();
    file_system_manager = new FileSystemManager();
    audio_settings = new AudioSettings();
    playlist = new Playlist("/");
    char* tmp = nullptr;
    if (char** tmp_ptr = &tmp; file_system_manager->getCurrentSongPath(tmp_ptr))
    {
        Serial.println(tmp);
        playlist->rollToSong(tmp);
    }

    delete[] tmp;

    display_handler->showSongScreen(playlist->getSongName(),
                                    BatteryManager::getBatteryVoltage(),
                                    audio_settings->getVolume());
}

// calls method of the current state of the machine, passes the buttonPress as a
// param
void DecisionMaker::performedAction(InputHandler::ButtonPress buttonPress)
{
    switch (deviceState)
    {
    case song_playing:
        songPlayingAction(buttonPress);
        break;
    case menu:

        break;
    case song_select:

        break;
    case settings:

        break;
    case debug:

        break;
    default:

        break;
    }
}
DisplayHandler* DecisionMaker::getDisplayHandler()
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
    case InputHandler::MiddleButtonLongPress:
        deviceState = menu;
        // TODO display show menu
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
    if (playlist->playlistLoop(audio_settings))
    {
        display_handler->changeSongName(playlist->getSongName());
        file_system_manager->setCurrentSongPath(playlist->getSongName());
    }
}


