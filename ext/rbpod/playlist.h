/* playlist.h */

#ifndef RBPOD_PLAYLIST_H
#define RBPOD_PLAYLIST_H

extern VALUE cRbPodPlaylist;

void Init_rbpod_playlist(void);

inline VALUE rbpod_playlist_wrap(Itdb_Playlist *playlist, gboolean freeable);

#endif /* RBPOD_PLAYLIST_H */
