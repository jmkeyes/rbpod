/* playlist.c */

#include "rbpod.h"
#include "track.h"
#include "playlist.h"
#include "collection.h"

VALUE cRbPodPlaylist;
VALUE mRbPodPlaylistCollection;

static VALUE rbpod_playlist_collection_parent(VALUE self) {
    return rb_iv_get(self, "@parent");;
}

inline VALUE rbpod_playlist_collection_create(VALUE parent, GList *items) {
    VALUE collection = rbpod_collection_create(items, cRbPodPlaylist);
    rb_extend_object(collection, mRbPodPlaylistCollection);
    rb_iv_set(collection, "@parent", parent);
    return collection;
}

static VALUE rbpod_playlist_is_podcast(VALUE self) {
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return BooleanValue(itdb_playlist_is_podcasts(playlist));
}

static VALUE rbpod_playlist_is_master(VALUE self) {
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return BooleanValue(itdb_playlist_is_mpl(playlist));
}

static VALUE rbpod_playlist_is_smart(VALUE self) {
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return BooleanValue(playlist->is_spl);
}

static VALUE rbpod_playlist_timestamp_get(VALUE self) {
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return rb_funcall(rb_cTime, rb_intern("at"), 1, INT2NUM(playlist->timestamp));
}

static VALUE rbpod_playlist_shuffle(VALUE self) {
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    itdb_playlist_randomize(playlist);
    return Qnil;
}

static VALUE rbpod_playlist_tracks_get(VALUE self) {
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return rbpod_track_collection_create(self, playlist->members);
}

static VALUE rbpod_playlist_length_get(VALUE self) {
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return INT2NUM(itdb_playlist_tracks_number(playlist));
}

static VALUE rbpod_playlist_name_get(VALUE self) {
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return rb_str_new2(playlist->name);
}

static VALUE rbpod_playlist_id_get(VALUE self) {
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return INT2NUM(playlist->id);
}

static VALUE rbpod_playlist_initialize(VALUE self) {
    /* Nothing to intiialize, yet. */
    return self;
}

static void rbpod_playlist_deallocate(void *handle) {
    itdb_playlist_free((Itdb_Playlist *) handle);
}

static VALUE rbpod_playlist_allocate(VALUE self) {
    Itdb_Playlist *playlist = itdb_playlist_new("Untitled Playlist", FALSE);
    return Data_Wrap_Struct(cRbPodPlaylist, NULL, rbpod_playlist_deallocate, (void *) playlist);
}

void Init_rbpod_playlist(void) {
    cRbPodPlaylist = rb_define_class_under(mRbPod, "Playlist", rb_cObject);

    rb_define_alloc_func(cRbPodPlaylist, rbpod_playlist_allocate);

    rb_define_method(cRbPodPlaylist, "initialize", rbpod_playlist_initialize, 0);

    rb_define_method(cRbPodPlaylist, "id", rbpod_playlist_id_get, 0);
    rb_define_method(cRbPodPlaylist, "name", rbpod_playlist_name_get, 0);
    rb_define_method(cRbPodPlaylist, "length", rbpod_playlist_length_get, 0);
    rb_define_method(cRbPodPlaylist, "tracks", rbpod_playlist_tracks_get, 0);
    rb_define_method(cRbPodPlaylist, "shuffle!", rbpod_playlist_shuffle, 0);
    rb_define_method(cRbPodPlaylist, "created_on", rbpod_playlist_timestamp_get, 0);

    rb_define_method(cRbPodPlaylist, "smart_playlist?", rbpod_playlist_is_smart, 0);
    rb_define_method(cRbPodPlaylist, "master_playlist?", rbpod_playlist_is_master, 0);
    rb_define_method(cRbPodPlaylist, "podcast_playlist?", rbpod_playlist_is_podcast, 0);

    mRbPodPlaylistCollection = rb_define_module_under(cRbPodPlaylist, "Collection");

    rb_define_private_method(mRbPodPlaylistCollection, "parent", rbpod_playlist_collection_parent, 0);

    return;
}

