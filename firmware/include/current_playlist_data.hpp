#pragma once

struct PlaylistData {
  char * m_current_playlist_root;
  char * m_current_playing;
  PlaylistData(char * current_playlist_root, char * current_playing);
};
