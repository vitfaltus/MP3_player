#include "decision_maker.hpp"


DecisionMaker::DecisionMaker()
{
    DeviceState = song_playing;

    displayHandler = new DisplayHandler();
    FileSystemManager = new C_FileSystemManager();
    AudioSettings = new C_AudioSettings();

    AudioSettings->setVolume(FileSystemManager->getDefaultVolume());

    displayHandler->setScreenTimeoutSeconds(FileSystemManager->getTimeoutTimeSeconds());

    displayHandler->initTopBanner(BatteryManager::getBatteryVoltage(),AudioSettings->getVolume(), FileSystemManager->isFsPresent());


    SongPlayer = new C_SongPlayer("/");

    // setting current song as the one saved in the .current_song
    char* tmp = nullptr;
    if (char** tmp_ptr = &tmp; FileSystemManager->getCurrentSongPath(tmp_ptr))
    {
        SongPlayer->rollToSong(tmp);
    }

    delete[] tmp;


    displayHandler->showSongScreen(SongPlayer->getSongName(),
                                    BatteryManager::getBatteryVoltage(),
                                    AudioSettings->getVolume());

    MenuSelectorPosition = 0;
    SongSelectSelectorPosition = 0;
    SettingsSelectorPosition = 0;
}

// calls method of the current state of the machine, passes the buttonPress as a param
void DecisionMaker::performedAction(const InputHandler::ButtonPress buttonPress)
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
    return displayHandler;
}

void DecisionMaker::songPlayingAction(const InputHandler::ButtonPress buttonPress)
{
    if (displayHandler->displayDimmingRoutine(buttonPress))
    {
        return;
    }

    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress: // volume down
        AudioSettings->volumeDown();
        displayHandler->changeVolumeLevel(AudioSettings->getVolume());
        break;
    case InputHandler::LeftButtonLongPress: // previous song
        SongPlayer->playPreviousSong(AudioSettings);
        displayHandler->changeSongName(SongPlayer->getSongName());
        FileSystemManager->setCurrentSongPath(SongPlayer->getSongName());
        break;
    case InputHandler::MiddleButtonPress: // pause/play song
        if (SongPlayer->isPaused())
        {
            SongPlayer->play(AudioSettings);
            AudioSettings->restoreAudio();
            displayHandler->drawPlay();
        }
        else
        {
            SongPlayer->stop();
            AudioSettings->shutAudio();
            displayHandler->drawPause();
        }
        break;
    case InputHandler::MiddleButtonLongPress: // change to menu
        SongPlayer->stop();
        AudioSettings->shutAudio();

        changeToMenu();
        break;
    case InputHandler::RightButtonPress: // volume up
        AudioSettings->volumeUp();
        displayHandler->changeVolumeLevel(AudioSettings->getVolume());
        break;
    case InputHandler::RightButtonLongPress: // next song
        SongPlayer->playNextSong(AudioSettings);
        displayHandler->changeSongName(SongPlayer->getSongName());
        FileSystemManager->setCurrentSongPath(SongPlayer->getSongName());
        break;
    default:
        break;
    }
    if (SongPlayer->playerLoop(AudioSettings))
    {
        displayHandler->changeSongName(SongPlayer->getSongName());
        FileSystemManager->setCurrentSongPath(SongPlayer->getSongName());
    }
}

void DecisionMaker::menuAction(const InputHandler::ButtonPress buttonPress)
{

    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress: // cycles menu selector up
        shiftMenuSelectorUp();
        break;
    case InputHandler::MiddleButtonPress: // selects menu element on the selector
        switch (MenuSelectorPosition) {
            case 0: // song selection
                DeviceState = song_select;

                SongSelectSelectorPosition = 0;
                fetchAndDisplaySongs();
            break;
            case 1: // settings
                changeToSettings();
            break;
            case 2: // debug
                DeviceState = debug;
                multi_heap_info_t info;
                displayHandler->drawDebugScreen(info);
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
    default:
        break;
    }
}

void DecisionMaker::songSelectAction(const InputHandler::ButtonPress buttonPress)
{
    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress:  // cycle up
        shiftSongSelectorUp();
        fetchAndDisplaySongs();

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
    default:
        break;
    }
}

void DecisionMaker::settingsAction(const InputHandler::ButtonPress buttonPress)
{
    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress:
        shiftSettingsSelectorUp();
        displayHandler->drawSettingsScreen(SettingsSelectorPosition);

        break;
    case InputHandler::MiddleButtonPress:
        switch (SettingsSelectorPosition)
        {
        case 0:
            DeviceState = settings_volume;
            displayHandler->drawSettingsDefaultVolume(FileSystemManager->getDefaultVolume());
            //changeToSettingVolume();
            break;
        case 1:
            DeviceState = settings_timeout;
            displayHandler->drawSettingsTimeOut(FileSystemManager->getTimeoutTimeSeconds());
            //changeToSettingTimeout();
            break;
        }
        break;
    case InputHandler::MiddleButtonLongPress:
        changeToMenu();
        break;
    case InputHandler::RightButtonPress:
        shiftSettingsSelectorDown();
        displayHandler->drawSettingsScreen(SettingsSelectorPosition);
        break;
    default:
        break;
    }
}

void DecisionMaker::settingsVolumeAction(const InputHandler::ButtonPress buttonPress)
{
    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress:
        AudioSettings->volumeDown();
        displayHandler->drawSettingsDefaultVolume(AudioSettings->getVolume());
        FileSystemManager->setDefaultVolume(AudioSettings->getVolume());
        break;
    case InputHandler::MiddleButtonPress:
        changeToSettings();
        break;
    case InputHandler::MiddleButtonLongPress:
        changeToSettings();
        break;
    case InputHandler::RightButtonPress:
        AudioSettings->volumeUp();
        displayHandler->drawSettingsDefaultVolume(AudioSettings->getVolume());
        FileSystemManager->setDefaultVolume(AudioSettings->getVolume());
        break;
    default:
        break;
    }
}

void DecisionMaker::settingsTimeoutAction(const InputHandler::ButtonPress buttonPress)
{
    switch (buttonPress)
    {
    case InputHandler::LeftButtonPress:
        displayHandler->decrementTimeout();
        displayHandler->drawSettingsTimeOut(displayHandler->getScreenTimeoutSeconds());
        FileSystemManager->setTimeoutTime(displayHandler->getScreenTimeoutSeconds());
        break;
    case InputHandler::MiddleButtonPress:
        changeToSettings();
        break;
    case InputHandler::MiddleButtonLongPress:
        changeToSettings();
        break;
    case InputHandler::RightButtonPress:
        displayHandler->incrementTimeout();
        displayHandler->drawSettingsTimeOut(displayHandler->getScreenTimeoutSeconds());
        FileSystemManager->setTimeoutTime(displayHandler->getScreenTimeoutSeconds());
        break;
    default:
        break;
    }
}

void DecisionMaker::debugAction(const InputHandler::ButtonPress buttonPress)
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
    displayHandler->drawMenuScreen(MenuSelectorPosition);

}

void DecisionMaker::shiftMenuSelectorDown()
{
    if (MenuSelectorPosition < 2) {
        MenuSelectorPosition++;
    }
    displayHandler->drawMenuScreen(MenuSelectorPosition);
}

void DecisionMaker::changeToMenu()
{
    DeviceState = menu;
    MenuSelectorPosition = 0;
    displayHandler->drawMenuScreen(MenuSelectorPosition);
}

void DecisionMaker::fetchAndDisplaySongs()
{
    if (!SongPlayer->getThreeSongNames(SongSelectSelectorPosition, DisplaySongArr))
    {
        displayHandler->displayErrorMessage("Song display error", 3);
        changeToMenu();
        return;
    }
    if (SongSelectSelectorPosition == 0)
    {
        displayHandler->drawSongSelectScreen(DisplaySongArr, 0);
    }
    else
    {
        displayHandler->drawSongSelectScreen(DisplaySongArr, 1);
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
    displayHandler->showSongScreen(SongPlayer->getSongName(),
                               BatteryManager::getBatteryVoltage(),
                               AudioSettings->getVolume());
    //reset the dimming timer
    displayHandler->displayDimmingRoutine(InputHandler::ButtonPress::MiddleButtonPress);
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

void DecisionMaker::changeToSettings()
{
    DeviceState = settings;
    displayHandler->drawSettingsScreen(SettingsSelectorPosition);
}


