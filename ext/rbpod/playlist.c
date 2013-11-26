/* playlist.c */

#include "rbpod.h"
#include "playlist.h"
#include "track_collection.h"

/*
 * call-seq:
 *     podcast?() -> Boolean
 *
 * Returns true or false if this playlist is the podcast-only playlist.
 */
static VALUE rbpod_playlist_podcast_p(VALUE self)
{
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return BooleanValue(itdb_playlist_is_podcasts(playlist));
}

/*
 * call-seq:
 *     master?() -> Boolean
 *
 * Returns true or false if this playlist is the master playlist.
 */
static VALUE rbpod_playlist_master_p(VALUE self)
{
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return BooleanValue(itdb_playlist_is_mpl(playlist));
}

/*
 * call-seq:
 *     smart?() -> Boolean
 *
 * Returns true or false if this playlist is a smart playlist.
 */
static VALUE rbpod_playlist_smart_p(VALUE self)
{
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return BooleanValue(playlist->is_spl);
}

/*
 * call-seq:
 *     timestamp() -> Time
 *     created_on() -> Time
 *
 * Returns a Time object representing the time that this playlist was created.
 */
static VALUE rbpod_playlist_timestamp_get(VALUE self)
{
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return rb_funcall(rb_cTime, rb_intern("at"), 1, INT2NUM(playlist->timestamp));
}

/*
 * call-seq:
 *     shuffle!() -> nil
 *
 * Shuffles the tracks in this playlist in place.
 */
static VALUE rbpod_playlist_shuffle_bang(VALUE self)
{
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    itdb_playlist_randomize(playlist);
    return Qnil;
}

/*
 * call-seq:
 *     tracks() -> RbPod::Collection
 *
 * Returns a collection of the tracks in this playlist.
 */
static VALUE rbpod_playlist_tracks_get(VALUE self)
{
    return rb_class_new_instance(1, &self, cRbPodTrackCollection);
}

/*
 * call-seq:
 *     length() -> Integer
 *
 * Returns the total number of tracks in this playlist.
 */
static VALUE rbpod_playlist_length_get(VALUE self)
{
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return INT2NUM(itdb_playlist_tracks_number(playlist));
}

/*
 * call-seq:
 *     name() -> String
 *
 * Returns the name of this playlist.
 */
static VALUE rbpod_playlist_name_get(VALUE self)
{
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return rb_str_new2(playlist->name);
}

/*
 * call-seq:
 *     <=>(other) -> Integer
 *
 * Compares two playlists based on their names.
 */
static VALUE rbpod_playlist_compare(VALUE self, VALUE other)
{
    VALUE this_playlist_name, other_playlist_name;

    if (rb_class_of(other) != cRbPodPlaylist) {
        rb_raise(eRbPodError, "Can't compare a Playlist with %s", StringValueCStr(other));
        return Qnil;
    }

    this_playlist_name  = rbpod_playlist_name_get(self);
    other_playlist_name = rbpod_playlist_name_get(other);

    return rb_str_cmp(this_playlist_name, other_playlist_name);
}

/*
 * call-seq:
 *     id() -> Fixnum
 *
 * Returns the internal ID number for this playlist. Do not use this method.
 */
static VALUE rbpod_playlist_id_get(VALUE self)
{
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    return INT2NUM(playlist->id);
}

/*
 * call-seq:
 *     initialize(name, smart_playlist) -> RbPod::Playlist
 *
 * Creates a detached playlist. (Not managed by the database.)
 */
static VALUE rbpod_playlist_initialize(int argc, VALUE *argv, VALUE self)
{
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    VALUE playlist_name, smart_playlist;
    gboolean _smart_playlist = FALSE;
    gchar *_playlist_name = NULL;

    if (rb_scan_args(argc, argv, "11", &playlist_name, &smart_playlist) < 1) {
        rb_raise(eRbPodError, "Invalid arguments.");
        return Qnil;
    }

    /* By default, playlists should not be smart. */
    _smart_playlist = (smart_playlist == Qtrue) ? TRUE : FALSE;

    /* Ensure we got a String-ish value, or bail. */
    _playlist_name = StringValueCStr(playlist_name);

    /* Free the old playlist. */
    itdb_playlist_free(playlist);

    /* Attach the newly created playlist to this instance. */
    DATA_PTR(self) = itdb_playlist_new(_playlist_name, _smart_playlist);

    /* Done. */
    return self;
}

static void rbpod_playlist_deallocate(void *handle)
{
    Itdb_Playlist *playlist = (Itdb_Playlist *) handle;

    /* This playlist was unmanaged, so free it manually. */
    if (playlist->itdb == NULL || playlist->id == 0) {
        itdb_playlist_free(playlist);
    }

    return;
}

static VALUE rbpod_playlist_allocate(VALUE self)
{
    Itdb_Playlist *playlist = itdb_playlist_new("KILROY WAS HERE", FALSE);
    return Data_Wrap_Struct(cRbPodPlaylist, NULL, rbpod_playlist_deallocate, (void *) playlist);
}

void Init_rbpod_playlist(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    cRbPodPlaylist = rb_define_class_under(mRbPod, "Playlist", rb_cObject);

    rb_define_alloc_func(cRbPodPlaylist, rbpod_playlist_allocate);

    rb_define_method(cRbPodPlaylist, "initialize", rbpod_playlist_initialize, -1);

    rb_define_private_method(cRbPodPlaylist, "id", rbpod_playlist_id_get, 0);

    rb_define_method(cRbPodPlaylist, "<=>", rbpod_playlist_compare, 1);

    rb_define_method(cRbPodPlaylist, "name", rbpod_playlist_name_get, 0);
    rb_define_method(cRbPodPlaylist, "length", rbpod_playlist_length_get, 0);
    rb_define_method(cRbPodPlaylist, "tracks", rbpod_playlist_tracks_get, 0);
    rb_define_method(cRbPodPlaylist, "shuffle!", rbpod_playlist_shuffle_bang, 0);
    rb_define_method(cRbPodPlaylist, "timestamp", rbpod_playlist_timestamp_get, 0);

    rb_define_alias(cRbPodPlaylist, "created_on", "timestamp");

    rb_define_method(cRbPodPlaylist, "smart?", rbpod_playlist_smart_p, 0);
    rb_define_method(cRbPodPlaylist, "master?", rbpod_playlist_master_p, 0);
    rb_define_method(cRbPodPlaylist, "podcast?", rbpod_playlist_podcast_p, 0);

    return;
}

