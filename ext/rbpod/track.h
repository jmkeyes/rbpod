/* track.h */

#ifndef RBPOD_TRACK_H
#define RBPOD_TRACK_H

extern VALUE cRbPodTrack;

void Init_rbpod_track(void);

inline VALUE rbpod_track_wrap(Itdb_Track *track, gboolean freeable);

#endif /* RBPOD_TRACK_H */
