/* track_collection.h */

#ifndef RBPOD_TRACK_COLLECTION_H
#define RBPOD_TRACK_COLLECTION_H

RUBY_EXTERN VALUE mRbPodTrackCollection;

void Init_rbpod_track_collection(void);

inline VALUE rbpod_track_collection_create(VALUE parent, GList *items);

#endif /* RBPOD_TRACK_COLLECTION_H */
