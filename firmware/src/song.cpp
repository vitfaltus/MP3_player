#include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "song.hpp"

Song::Song(const char* song_path){
    m_song_path = strdup(song_path);
    m_mp3 = nullptr;
    m_file = nullptr;
}

Song::~Song(){
    free_buffer();

    if (m_song_path){
        free(m_song_path);
    }

    if (m_next_song){
        m_next_song->set_previous_song(nullptr);
    }
    if (m_previous_song){
        m_previous_song->set_previous_song(nullptr);
    }
}

Song* Song::get_next_song(){
    return m_next_song;
}

Song* Song::get_previous_song(){
    return m_previous_song;
}

void Song::set_previous_song(Song* song_ptr){
    m_previous_song = song_ptr;
}

void Song::set_next_song(Song* song_ptr){
    m_next_song = song_ptr;
}

bool Song::is_playing(){
    if (m_mp3->isRunning()){
        if (!m_mp3->loop()){
            m_mp3->stop();
            return false;
        }
        return true;
    }
    return false;
}

void Song::stop(){
    if (m_file && m_mp3){
        m_mp3->stop();
        m_file->close();
        delete m_mp3;
    }
}

void Song::play(AudioOutputI2S* audio_output){
    if (!m_file){
        m_file = new AudioFileSourceSD(m_song_path);
    }
    if (!m_mp3){
        m_mp3 = new AudioGeneratorMP3();
    }
    Serial.println("Starting again");
    m_mp3->begin(m_file, audio_output);
}

char* Song::get_song_path(){
    return m_song_path;
}

void Song::free_buffer(){
    if (m_file){
        delete m_file;
    }
    if (m_mp3){
        delete m_mp3;
    }
}
