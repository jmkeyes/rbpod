/* track.h */

#ifndef RBPOD_TRACK_H
#define RBPOD_TRACK_H

extern VALUE cRbPodTrack;
extern VALUE mRbPodTrackCollection;

void Init_rbpod_track(void);

inline VALUE rbpod_track_collection_create(VALUE parent, GList *items);

#endif /* RBPOD_TRACK_H */
