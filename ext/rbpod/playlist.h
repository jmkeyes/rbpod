/* playlist.h */

#ifndef RBPOD_PLAYLIST_H
#define RBPOD_PLAYLIST_H

extern VALUE cRbPodPlaylist;
extern VALUE mRbPodPlaylistCollection;

void Init_rbpod_playlist(void);

inline VALUE rbpod_playlist_collection_create(VALUE parent, GList *items);

#endif /* RBPOD_PLAYLIST_H */
