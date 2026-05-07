#include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"

#include "AudioOutputI2S.h"


class Song{
    AudioFileSource* m_file;
    AudioGeneratorMP3* m_mp3;
    char* m_song_path;

    Song* m_previous_song;
    Song* m_next_song;

    public:

    Song(char* song_path){
        memcpy(m_song_path, song_path, strlen(song_path));
    }

    ~Song(){
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

    Song* get_next_song(){
        return m_next_song;
    }

    Song* get_previous_song(){
        return m_previous_song;
    }

    void set_previous_song(Song* song_ptr){
        m_previous_song = song_ptr;
    }
    void set_next_song(Song* song_ptr){
        m_next_song = song_ptr;
    }

    bool is_playing(){

    }

    void stop(){

    }

    void play(AudioOutputI2S* audio_output){
        if (!m_file){
            m_file = new AudioFileSourceSD(m_song_path);
        }
        if (m_mp3){
            m_mp3 = new AudioGeneratorMP3();
        }
        m_mp3->begin(m_file, audio_output);
    }

    void free_buffer(){
        if (m_file){
            delete m_file;
        }
        if (m_mp3){
            delete m_mp3;
        }
    }
    
};