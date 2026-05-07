#include "song.hpp"

class Playlist{

    Song* m_current_song;

    bool m_paused;

    char* add_dir_slash(const char* s){
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

    public:

    Playlist() {
        m_paused = true;
    }

    ~Playlist(){
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

    void AddSong(const char* path){
        char* slash_name = add_dir_slash(path);
      if (!slash_name) {return;}

      Song* new_song = new Song(slash_name);
      Serial.println("Song created");

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

    void Play();
    void Stop();
    void PlaylistLoop();
    void PlayNextSong();
    void PlayPreviousSong();

};