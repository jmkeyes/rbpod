/* track_collection.c */

#include "rbpod.h"
#include "track.h"
#include "collection.h"
#include "track_collection.h"

/*
 * call-seq:
 *     initialize(playlist) -> RbPod::TrackCollection
 *
 * Given an RbPod::Playlist +playlist+, returns a collection of tracks within the playlist.
 */
static VALUE rbpod_track_collection_initialize(VALUE self, VALUE playlist)
{
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

    return;
}

