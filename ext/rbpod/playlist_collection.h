/* playlist_collection.h */

#ifndef RBPOD_PLAYLIST_COLLECTION_H
#define RBPOD_PLAYLIST_COLLECTION_H

#include "playlist.h"

RUBY_EXTERN VALUE mRbPodPlaylistCollection;

void Init_rbpod_playlist_collection(void);

inline VALUE rbpod_playlist_collection_create(VALUE parent, GList *items);

#endif /* RBPOD_PLAYLIST_COLLECTION_H */
