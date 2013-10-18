/* playlist.c */

#include "rbpod.h"
#include "track.h"
#include "playlist.h"

VALUE cRbPodPlaylist;

static inline Itdb_Playlist *rbpod_playlist_unwrap(VALUE self) {
    Itdb_Playlist *playlist = NULL;
    Data_Get_Struct(self, Itdb_Playlist, playlist);
    return playlist;
}

static VALUE rbpod_playlist_is_podcast(VALUE self) {
    Itdb_Playlist *playlist = rbpod_playlist_unwrap(self);
    return BooleanValue(itdb_playlist_is_podcasts(playlist));
}

static VALUE rbpod_playlist_is_master(VALUE self) {
    Itdb_Playlist *playlist = rbpod_playlist_unwrap(self);
    return BooleanValue(itdb_playlist_is_mpl(playlist));
}

static VALUE rbpod_playlist_is_smart(VALUE self) {
    Itdb_Playlist *playlist = rbpod_playlist_unwrap(self);
    return BooleanValue(playlist->is_spl);
}

static VALUE rbpod_playlist_tracks_get(VALUE self) {
    Itdb_Playlist *playlist = rbpod_playlist_unwrap(self);
    return rbpod_collection_wrap(playlist->members, cRbPodTrack, FALSE);
}

static VALUE rbpod_playlist_length_get(VALUE self) {
    Itdb_Playlist *playlist = rbpod_playlist_unwrap(self);
    return INT2NUM(playlist->num);
}

static VALUE rbpod_playlist_name_get(VALUE self) {
    Itdb_Playlist *playlist = rbpod_playlist_unwrap(self);
    return rb_str_new2(playlist->name);
}

static VALUE rbpod_playlist_id_get(VALUE self) {
    Itdb_Playlist *playlist = rbpod_playlist_unwrap(self);
    return INT2FIX(playlist->id);
}

static VALUE rbpod_playlist_initialize(VALUE self) {
    return self;
}

static void rbpod_playlist_deallocate(void *handle) {
    itdb_playlist_free((Itdb_Playlist *) handle);
}

inline VALUE rbpod_playlist_wrap(Itdb_Playlist *playlist, gboolean freeable) {
    return Data_Wrap_Struct(cRbPodPlaylist, NULL, freeable ? rbpod_playlist_deallocate : NULL, (void *) playlist);
}

static VALUE rbpod_playlist_allocate(VALUE self) {
    Itdb_Playlist *playlist = itdb_playlist_new("Untitled Playlist", FALSE);
    return rbpod_playlist_wrap(playlist, TRUE);
}

void Init_rbpod_playlist(void) {
    cRbPodPlaylist = rb_define_class_under(mRbPod, "Playlist", rb_cObject);

    rb_define_alloc_func(cRbPodPlaylist, rbpod_playlist_allocate);

    rb_define_method(cRbPodPlaylist, "initialize", rbpod_playlist_initialize, 0);

    rb_define_method(cRbPodPlaylist, "id", rbpod_playlist_id_get, 0);
    rb_define_method(cRbPodPlaylist, "name", rbpod_playlist_name_get, 0);
    rb_define_method(cRbPodPlaylist, "length", rbpod_playlist_length_get, 0);
    rb_define_method(cRbPodPlaylist, "tracks", rbpod_playlist_tracks_get, 0);

    rb_define_method(cRbPodPlaylist, "smart_playlist?", rbpod_playlist_is_smart, 0);
    rb_define_method(cRbPodPlaylist, "master_playlist?", rbpod_playlist_is_master, 0);
    rb_define_method(cRbPodPlaylist, "podcast_playlist?", rbpod_playlist_is_podcast, 0);

    return;
}

