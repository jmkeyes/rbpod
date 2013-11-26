/* track_collection.c */

#include "rbpod.h"
#include "track.h"
#include "collection.h"
#include "track_collection.h"

/*
 * call-seq:
 *     playlist() -> RbPod::Playlist
 *
 * Returns the playlist this track collection is attached to.
 */
static VALUE rbpod_track_collection_playlist(VALUE self)
{
    return rb_iv_get(self, "@playlist");
}

/*
 * call-seq:
 *     initialize(playlist) -> RbPod::TrackCollection
 *
 * Given an RbPod::Playlist +playlist+, returns a collection of tracks within the playlist.
 */
static VALUE rbpod_track_collection_initialize(VALUE self, VALUE playlist)
{
    rb_iv_set(self, "@playlist", playlist);
    return self;
}

void Init_rbpod_track_collection(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    cRbPodTrackCollection = rb_define_class_under(mRbPod, "TrackCollection", rb_cObject);

    rb_include_module(cRbPodTrackCollection, mRbPodCollection);

    rb_define_method(cRbPodTrackCollection, "initialize", rbpod_track_collection_initialize, 1);

    rb_define_method(cRbPodTrackCollection, "playlist", rbpod_track_collection_playlist, 0);

    return;
}

