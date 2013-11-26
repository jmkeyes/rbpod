/* track_collection.c */

#include "rbpod.h"
#include "track.h"
#include "playlist.h"
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
    Itdb_Playlist *_playlist = TYPED_DATA_PTR(playlist, Itdb_Playlist);

    if (rb_obj_is_instance_of(playlist, cRbPodPlaylist) == FALSE) {
        rb_raise(eRbPodError, "Invalid Arguments: Expected RbPod::Playlist, got %s", StringValueCStr(playlist));
        return Qnil;
    }

    rb_iv_set(self, "@playlist", playlist);

    DATA_PTR(self) = _playlist->members;

    return self;
}

static VALUE rbpod_track_collection_type(VALUE self)
{
    return cRbPodTrack;
}

void Init_rbpod_track_collection(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    cRbPodTrackCollection = rb_define_class_under(mRbPod, "TrackCollection", rb_cObject);

    rb_real_include_module(cRbPodTrackCollection, mRbPodCollection);

    rb_define_method(cRbPodTrackCollection, "initialize", rbpod_track_collection_initialize, 1);

    rb_define_private_method(cRbPodTrackCollection, "type", rbpod_track_collection_type, 0);

    rb_define_method(cRbPodTrackCollection, "playlist", rbpod_track_collection_playlist, 0);

    return;
}

