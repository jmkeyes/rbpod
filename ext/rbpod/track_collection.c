/* track.c */

#include "rbpod.h"
#include "track.h"
#include "collection.h"

VALUE mRbPodTrackCollection;

static VALUE rbpod_track_collection_parent(VALUE self)
{
    return rb_iv_get(self, "@parent");
}

inline VALUE rbpod_track_collection_create(VALUE parent, GList *items)
{
    VALUE collection = rbpod_collection_create(items, cRbPodTrack);
    rb_extend_object(collection, mRbPodTrackCollection);
    rb_extend_object(collection, rb_mComparable);
    rb_iv_set(collection, "@parent", parent);
    return collection;
}

void Init_rbpod_track_collection(void)
{
    mRbPodTrackCollection = rb_define_module_under(mRbPod, "TrackCollection");

    rb_define_private_method(mRbPodTrackCollection, "parent", rbpod_track_collection_parent, 0);

    return;
}

