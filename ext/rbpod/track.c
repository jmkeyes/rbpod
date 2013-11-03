/* track.c */

#include "rbpod.h"
#include "track.h"
#include "collection.h"

VALUE cRbPodTrack;
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

static VALUE rbpod_track_transferred_p(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return BooleanValue(track->transferred);
}

static VALUE rbpod_track_ipod_path_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return rb_str_new2(track->ipod_path);
}

static VALUE rbpod_track_file_type_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return rb_str_new2(track->filetype);
}

static VALUE rbpod_track_artist_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return rb_str_new2(track->artist);
}

static VALUE rbpod_track_album_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return rb_str_new2(track->album);
}

static VALUE rbpod_track_title_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return rb_str_new2(track->title);
}

static VALUE rbpod_track_id_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return rb_str_new2(track->filetype);
}

static VALUE rbpod_track_initialize(VALUE self)
{
    return self;
}

static void rbpod_track_deallocate(void *handle)
{
    itdb_track_free((Itdb_Track *) handle);
    return;
}

static VALUE rbpod_track_allocate(VALUE self)
{
    Itdb_Track *track = itdb_track_new();
    return Data_Wrap_Struct(cRbPodTrack, NULL, rbpod_track_deallocate, (void *) track);
}

void Init_rbpod_track(void)
{
    cRbPodTrack = rb_define_class_under(mRbPod, "Track", rb_cObject);

    rb_define_alloc_func(cRbPodTrack, rbpod_track_allocate);

    rb_define_method(cRbPodTrack, "initialize", rbpod_track_initialize, 0);

    rb_define_method(cRbPodTrack, "id", rbpod_track_id_get, 0);
    rb_define_method(cRbPodTrack, "title", rbpod_track_title_get, 0);
    rb_define_method(cRbPodTrack, "album", rbpod_track_album_get, 0);
    rb_define_method(cRbPodTrack, "artist", rbpod_track_artist_get, 0);
    rb_define_method(cRbPodTrack, "file_type", rbpod_track_file_type_get, 0);
    rb_define_method(cRbPodTrack, "ipod_path", rbpod_track_ipod_path_get, 0);

    rb_define_method(cRbPodTrack, "transferred?", rbpod_track_transferred_p, 0);

    mRbPodTrackCollection = rb_define_module_under(cRbPodTrack, "Collection");

    rb_define_private_method(mRbPodTrackCollection, "parent", rbpod_track_collection_parent, 0);

    return;
}

