#include <SD.h>
#include "playlist.hpp"

char* Playlist::add_dir_slash(const char* s){
    size_t len = strlen(s);

    char* out = (char*)malloc(len + 2); 
    if (!out) {
        Serial.println("Path name not allocated");
        return nullptr;
    }

    out[0] = '/';
    memcpy(out + 1, s, len + 1); 

    return out;
}

void Playlist::CreateSequentialPlaylist(const char* path){
    m_root_path = strdup(path);
    File root_path_file = SD.open(path);
    CreatePlaylist(root_path_file);
    Serial.println("Playlist created");
}



Playlist::Playlist(const char* path) {
    m_current_song = nullptr;
    m_root_path = nullptr;
    this->CreateSequentialPlaylist(path);
    m_paused = false;
}

Playlist::~Playlist(){
    if (!m_current_song){
        return;
    }
    while (m_current_song->get_previous_song()){
        m_current_song = m_current_song->get_next_song();
    }
    Song* next_song = m_current_song->get_next_song();
    while (m_current_song){
        m_current_song->~Song();
        m_current_song = next_song;
        next_song = m_current_song->get_next_song();
    }
}



void Playlist::AddSong(const char* path){
    char* slash_name = add_dir_slash(path); 
    if (!slash_name) {return;}

    Song* new_song = new Song(slash_name);
    Serial.print("Song created: ");
    Serial.println(new_song->get_song_path());
      

    if (m_current_song == nullptr){
        m_current_song = new_song;
    }
      
    else{

        while(m_current_song->get_next_song() != nullptr){
            m_current_song = m_current_song->get_next_song();
        }

        m_current_song->set_next_song(new_song);
        new_song->set_previous_song(m_current_song);
    }
}

void Playlist::Play(){
    m_paused = false;
}

void Playlist::Stop(){
    m_paused = true;
}

bool Playlist::IsPaused(){
    return m_paused;
}

void Playlist::PlaylistLoop(){
    if (!m_paused){
        if (!m_current_song->is_playing()){
            PlayNextSong();
        }
    }
}

void Playlist::PlayNextSong(){
    if (m_current_song->get_next_song()){
        m_current_song = m_current_song->get_next_song();
    }
}

void Playlist::PlayPreviousSong(){
    if (m_current_song->get_previous_song()){
        m_current_song = m_current_song->get_previous_song();
    }
}

void Playlist::CreatePlaylist(File& current_dir){
    while(true){
        File entry = current_dir.openNextFile();
        if (!entry){
            return;
        }
        if (entry.isDirectory()){
            continue;
        }
        else{     
            this->AddSong(entry.name());
        }
    }
}
