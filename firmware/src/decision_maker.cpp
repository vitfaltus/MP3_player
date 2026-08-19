#include "decision_maker.hpp"


DecisionMaker::DecisionMaker()
{
    DeviceState = song_playing;

    DisplayHandler = new C_DisplayHandler();
    FileSystemManager = new C_FileSystemManager();
    AudioSettings = new C_AudioSettings();
    SongPlayer = new C_SongPlayer("/");

    // setting current song as the one saved in the .current_song
    char* tmp = nullptr;
    if (char** tmp_ptr = &tmp; FileSystemManager->getCurrentSongPath(tmp_ptr))
    {
        SongPlayer->rollToSong(tmp);
    }

    delete[] tmp;

    AudioSettings->setVolume(FileSystemManager->getDefaultVolume());

    DisplayHandler->setScreenTimeoutSeconds(FileSystemManager->getTimeoutTimeSeconds());

    DisplayHandler->showSongScreen(SongPlayer->getSongName(),
                                    BatteryManager::getBatteryVoltage(),
                                    AudioSettings->getVolume());

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

C_DisplayHandler* DecisionMaker::getDisplayHandler() const
{
    return DisplayHandler;
}

void DecisionMaker::songPlayingAction(InputHandler::ButtonPress buttonPress)
{
    if (DisplayHandler->displayDimmingRoutine(buttonPress))
    {
        return;
    }

    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress: // volume down
        AudioSettings->volumeDown();
        DisplayHandler->changeVolumeLevel(AudioSettings->getVolume());
        break;
    case InputHandler::LeftButtonLongPress: // previous song
        SongPlayer->playPreviousSong(AudioSettings);
        DisplayHandler->changeSongName(SongPlayer->getSongName());
        FileSystemManager->setCurrentSongPath(SongPlayer->getSongName());
        break;
    case InputHandler::MiddleButtonPress: // pause/play song
        if (SongPlayer->isPaused())
        {
            SongPlayer->play(AudioSettings);
            AudioSettings->restoreAudio();
            DisplayHandler->drawPlay();
        }
        else
        {
            SongPlayer->stop();
            AudioSettings->shutAudio();
            DisplayHandler->drawPause();
        }
        break;
    case InputHandler::MiddleButtonLongPress: // change to menu
        SongPlayer->stop();
        AudioSettings->shutAudio();

        changeToMenu();
        break;
    case InputHandler::RightButtonPress: // volume up
        AudioSettings->volumeUp();
        DisplayHandler->changeVolumeLevel(AudioSettings->getVolume());
        break;
    case InputHandler::RightButtonLongPress: // next song
        SongPlayer->playNextSong(AudioSettings);
        DisplayHandler->changeSongName(SongPlayer->getSongName());
        FileSystemManager->setCurrentSongPath(SongPlayer->getSongName());
        break;
    default:
        break;
    }
    if (SongPlayer->playerLoop(AudioSettings))
    {
        DisplayHandler->changeSongName(SongPlayer->getSongName());
        FileSystemManager->setCurrentSongPath(SongPlayer->getSongName());
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
                DisplayHandler->drawSettingsScreen(SettingsSelectorPosition);
            break;
            case 2: // debug
                DeviceState = debug;
                multi_heap_info_t info;
                DisplayHandler->drawDebugScreen(info);
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
            SongPlayer->rollToSong(DisplaySongArr[0]);
        }
        else
        {
            SongPlayer->rollToSong(DisplaySongArr[1]);
        }
        changeToSongPlaying();
        FileSystemManager->setCurrentSongPath(SongPlayer->getSongName());
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
        DisplayHandler->drawSettingsScreen(SettingsSelectorPosition);

        break;
    case InputHandler::LeftButtonLongPress:

        break;
    case InputHandler::MiddleButtonPress:
        switch (SettingsSelectorPosition)
        {
        case 0:
            DeviceState = settings_volume;
            DisplayHandler->drawSettingsDefaultVolume(FileSystemManager->getDefaultVolume());
            //changeToSettingVolume();
            break;
        case 1:
            DeviceState = settings_timeout;
            DisplayHandler->drawSettingsTimeOut(FileSystemManager->getTimeoutTimeSeconds());
            //changeToSettingTimeout();
            break;
        }
        break;
    case InputHandler::MiddleButtonLongPress:
        changeToMenu();
        break;
    case InputHandler::RightButtonPress:
        shiftSettingsSelectorDown();
        DisplayHandler->drawSettingsScreen(SettingsSelectorPosition);
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
        AudioSettings->volumeDown();
        DisplayHandler->drawSettingsDefaultVolume(AudioSettings->getVolume());
        FileSystemManager->setDefaultVolume(AudioSettings->getVolume());
        break;
    case InputHandler::MiddleButtonPress:
        changeToMenu();
        break;
    case InputHandler::MiddleButtonLongPress:
        changeToMenu();
        break;
    case InputHandler::RightButtonPress:
        AudioSettings->volumeUp();
        DisplayHandler->drawSettingsDefaultVolume(AudioSettings->getVolume());
        FileSystemManager->setDefaultVolume(AudioSettings->getVolume());
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
        DisplayHandler->decrementTimeout();
        DisplayHandler->drawSettingsTimeOut(DisplayHandler->getScreenTimeoutSeconds());
        FileSystemManager->setTimeoutTime(DisplayHandler->getScreenTimeoutSeconds());
        break;
    case InputHandler::MiddleButtonPress:
        changeToMenu();
        break;
    case InputHandler::MiddleButtonLongPress:
        changeToMenu();
        break;
    case InputHandler::RightButtonPress:
        DisplayHandler->incrementTimeout();
        DisplayHandler->drawSettingsTimeOut(DisplayHandler->getScreenTimeoutSeconds());
        FileSystemManager->setTimeoutTime(DisplayHandler->getScreenTimeoutSeconds());
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
    DisplayHandler->drawMenuScreen(MenuSelectorPosition);

}

void DecisionMaker::shiftMenuSelectorDown()
{
    if (MenuSelectorPosition < 2) {
        MenuSelectorPosition++;
    }
    DisplayHandler->drawMenuScreen(MenuSelectorPosition);
}

void DecisionMaker::changeToMenu()
{
    DeviceState = menu;
    MenuSelectorPosition = 0;
    DisplayHandler->drawMenuScreen(MenuSelectorPosition);
}

void DecisionMaker::fetchAndDisplaySongs()
{
    if (!SongPlayer->getThreeSongNames(SongSelectSelectorPosition, DisplaySongArr))
    {
        DisplayHandler->displayErrorMessage("Song display error", 3);
        changeToMenu();
        return;
    }
    if (SongSelectSelectorPosition == 0)
    {
        DisplayHandler->drawSongSelectScreen(DisplaySongArr, 0);
    }
    else
    {
        DisplayHandler->drawSongSelectScreen(DisplaySongArr, 1);
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
    DisplayHandler->showSongScreen(SongPlayer->getSongName(),
                               BatteryManager::getBatteryVoltage(),
                               AudioSettings->getVolume());
    //reset the dimming timer
    DisplayHandler->displayDimmingRoutine(InputHandler::ButtonPress::MiddleButtonPress);
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


