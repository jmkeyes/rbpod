/* track_collection.c */

#include "rbpod.h"
#include "track.h"
#include "collection.h"
#include "track_collection.h"

/*
 * call-seq:
 *     playlist() -> RbPod::Playlist
 *
 * The playlist from which this collection of tracks is attached to.
 */
static VALUE rbpod_track_collection_playlist(VALUE self)
{
    return rb_iv_get(self, "@pkaylist");
}

inline VALUE rbpod_track_collection_create(VALUE playlist, GList *items)
{
    VALUE collection = rbpod_collection_create(items, cRbPodTrack);
    rb_extend_object(collection, mRbPodTrackCollection);
    rb_iv_set(collection, "@playlist", playlist);
    return collection;
}

void Init_rbpod_track_collection(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    /* This module extends any collection of tracks at runtime. */
    mRbPodTrackCollection = rb_define_module_under(mRbPod, "TrackCollection");

    rb_define_method(mRbPodTrackCollection, "playlist", rbpod_track_collection_playlist, 0);

    return;
}

