#include "decision_maker.hpp"


DecisionMaker::DecisionMaker()
{
    DeviceState = song_playing;

    display_handler = new DisplayHandler();
    file_system_manager = new FileSystemManager();
    audio_settings = new AudioSettings();
    song_player = new SongPlayer("/");

    // setting current song as the one saved in the .current_song
    char* tmp = nullptr;
    if (char** tmp_ptr = &tmp; file_system_manager->getCurrentSongPath(tmp_ptr))
    {
        song_player->rollToSong(tmp);
    }

    delete[] tmp;

    audio_settings->setVolume(file_system_manager->getDefaultVolume());

    display_handler->setScreenTimeoutSeconds(file_system_manager->getTimeoutTimeSeconds());

    display_handler->showSongScreen(song_player->getSongName(),
                                    BatteryManager::getBatteryVoltage(),
                                    audio_settings->getVolume());

    MenuSelectorPosition = 0;
    SongSelectSelectorPosition = 0;
    SettingsSelectorPosition = 0;
}

// calls method of the current state of the machine, passes the buttonPress as a param
void DecisionMaker::performedAction(InputHandler::ButtonPress buttonPress)
{
    switch (DeviceState)
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
    case settings_volume:
        settingsVolumeAction(buttonPress);
        break;
    case settings_timeout:
        settingsTimeoutAction(buttonPress);
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
        song_player->playPreviousSong(audio_settings);
        display_handler->changeSongName(song_player->getSongName());
        file_system_manager->setCurrentSongPath(song_player->getSongName());
        break;
    case InputHandler::MiddleButtonPress: // pause/play song
        if (song_player->isPaused())
        {
            song_player->play(audio_settings);
            audio_settings->restoreAudio();
            display_handler->drawPlay();
        }
        else
        {
            song_player->stop();
            audio_settings->shutAudio();
            display_handler->drawPause();
        }
        break;
    case InputHandler::MiddleButtonLongPress: // change to menu
        song_player->stop();
        audio_settings->shutAudio();

        changeToMenu();
        break;
    case InputHandler::RightButtonPress: // volume up
        audio_settings->volumeUp();
        display_handler->changeVolumeLevel(audio_settings->getVolume());
        break;
    case InputHandler::RightButtonLongPress: // next song
        song_player->playNextSong(audio_settings);
        display_handler->changeSongName(song_player->getSongName());
        file_system_manager->setCurrentSongPath(song_player->getSongName());
        break;
    default:
        break;
    }
    if (song_player->playerLoop(audio_settings))
    {
        display_handler->changeSongName(song_player->getSongName());
        file_system_manager->setCurrentSongPath(song_player->getSongName());
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
                DeviceState = song_select;

                SongSelectSelectorPosition = 0;
                fetchAndDisplaySongs();
            break;
            case 1: // settings
                DeviceState = settings;

                //changeToSettings();
                display_handler->drawSettingsScreen(SettingsSelectorPosition);
            break;
            case 2: // debug
                DeviceState = debug;
                multi_heap_info_t info;
                display_handler->drawDebugScreen(info);
            break;

            default:
            break;
        }
        break;
    case InputHandler::MiddleButtonLongPress: // back to song playing
        changeToSongPlaying();


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

void DecisionMaker::songSelectAction(InputHandler::ButtonPress buttonPress)
{
    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress:  // cycle up
        shiftSongSelectorUp();
        fetchAndDisplaySongs();

        break;
    case InputHandler::LeftButtonLongPress:

        break;
    case InputHandler::MiddleButtonPress: // select song
        if (SongSelectSelectorPosition == 0)
        {
            song_player->rollToSong(DisplaySongArr[0]);
        }
        else
        {
            song_player->rollToSong(DisplaySongArr[1]);
        }
        changeToSongPlaying();
        file_system_manager->setCurrentSongPath(song_player->getSongName());
        break;
    case InputHandler::MiddleButtonLongPress: // back to menu
        changeToMenu();
        break;
    case InputHandler::RightButtonPress: // cycle down
        shiftSongSelectorDown();
        fetchAndDisplaySongs();
        break;
    case InputHandler::RightButtonLongPress:

        break;
    default:
        break;
    }
}

void DecisionMaker::settingsAction(InputHandler::ButtonPress buttonPress)
{
    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress:
        shiftSettingsSelectorUp();
        display_handler->drawSettingsScreen(SettingsSelectorPosition);

        break;
    case InputHandler::LeftButtonLongPress:

        break;
    case InputHandler::MiddleButtonPress:
        switch (SettingsSelectorPosition)
        {
        case 0:
            DeviceState = settings_volume;
            display_handler->drawSettingsDefaultVolume(file_system_manager->getDefaultVolume());
            //changeToSettingVolume();
            break;
        case 1:
            DeviceState = settings_timeout;
            display_handler->drawSettingsTimeOut(file_system_manager->getTimeoutTimeSeconds());
            //changeToSettingTimeout();
            break;
        }
        break;
    case InputHandler::MiddleButtonLongPress:
        changeToMenu();
        break;
    case InputHandler::RightButtonPress:
        shiftSettingsSelectorDown();
        display_handler->drawSettingsScreen(SettingsSelectorPosition);
        break;
    case InputHandler::RightButtonLongPress:

        break;
    default:
        break;
    }
}

void DecisionMaker::settingsVolumeAction(InputHandler::ButtonPress buttonPress)
{
    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress:
        audio_settings->volumeDown();
        display_handler->drawSettingsDefaultVolume(audio_settings->getVolume());
        file_system_manager->setDefaultVolume(audio_settings->getVolume());
        break;
    case InputHandler::MiddleButtonPress:
        changeToMenu();
        break;
    case InputHandler::MiddleButtonLongPress:
        changeToMenu();
        break;
    case InputHandler::RightButtonPress:
        audio_settings->volumeUp();
        display_handler->drawSettingsDefaultVolume(audio_settings->getVolume());
        file_system_manager->setDefaultVolume(audio_settings->getVolume());
        break;
    default:
        break;
    }
}

void DecisionMaker::settingsTimeoutAction(InputHandler::ButtonPress buttonPress)
{
    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress:
        display_handler->decrementTimeout();
        display_handler->drawSettingsTimeOut(display_handler->getScreenTimeoutSeconds());
        file_system_manager->setTimeoutTime(display_handler->getScreenTimeoutSeconds());
        break;
    case InputHandler::MiddleButtonPress:
        changeToMenu();
        break;
    case InputHandler::MiddleButtonLongPress:
        changeToMenu();
        break;
    case InputHandler::RightButtonPress:
        display_handler->incrementTimeout();
        display_handler->drawSettingsTimeOut(display_handler->getScreenTimeoutSeconds());
        file_system_manager->setTimeoutTime(display_handler->getScreenTimeoutSeconds());
        break;
    default:
        break;
    }
}

void DecisionMaker::debugAction(InputHandler::ButtonPress buttonPress)
{
    if (buttonPress != InputHandler::None) {
        changeToMenu();
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

void DecisionMaker::changeToMenu()
{
    DeviceState = menu;
    MenuSelectorPosition = 0;
    display_handler->drawMenuScreen(MenuSelectorPosition);
}

void DecisionMaker::fetchAndDisplaySongs()
{
    if (!song_player->getThreeSongNames(SongSelectSelectorPosition, DisplaySongArr))
    {
        display_handler->displayErrorMessage("Song display error", 3);
        changeToMenu();
        return;
    }
    if (SongSelectSelectorPosition == 0)
    {
        display_handler->drawSongSelectScreen(DisplaySongArr, 0);
    }
    else
    {
        display_handler->drawSongSelectScreen(DisplaySongArr, 1);
    }

}

void DecisionMaker::shiftSongSelectorUp()
{
    if (SongSelectSelectorPosition > 0)
    {
        SongSelectSelectorPosition--;
    }

}

void DecisionMaker::shiftSongSelectorDown()
{
    SongSelectSelectorPosition++;
}

void DecisionMaker::changeToSongPlaying()
{
    DeviceState = song_playing;
    display_handler->showSongScreen(song_player->getSongName(),
                               BatteryManager::getBatteryVoltage(),
                               audio_settings->getVolume());
    //reset the dimming timer
    display_handler->displayDimmingRoutine(InputHandler::ButtonPress::MiddleButtonPress);
}

void DecisionMaker::shiftSettingsSelectorUp()
{
    if (SettingsSelectorPosition > 0)
    {
        SettingsSelectorPosition--;
    }

}

void DecisionMaker::shiftSettingsSelectorDown()
{
    if (SettingsSelectorPosition < 1)
    {
        SettingsSelectorPosition++;
    }
}


