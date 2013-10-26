/* playlist.h */

#ifndef RBPOD_PLAYLIST_H
#define RBPOD_PLAYLIST_H

RUBY_EXTERN VALUE cRbPodPlaylist;
RUBY_EXTERN VALUE mRbPodPlaylistCollection;

void Init_rbpod_playlist(void);

inline VALUE rbpod_playlist_collection_create(VALUE parent, GList *items);

#endif /* RBPOD_PLAYLIST_H */
