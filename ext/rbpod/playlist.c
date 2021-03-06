/* playlist.c */

#include "rbpod.h"

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
 *     smart=(flag) -> nil
 *
 * Set this playlist to a smart playlist.
 */
static VALUE rbpod_playlist_smart_set(VALUE self, VALUE flag)
{
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);

    playlist->is_spl = (RTEST(flag)) ? TRUE : FALSE;

    return Qnil;
}

/*
 * call-seq:
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
 *     name=(string) -> nil
 *
 * Sets the name of this playlist.
 */
static VALUE rbpod_playlist_name_set(VALUE self, VALUE name)
{
    Itdb_Playlist *playlist = TYPED_DATA_PTR(self, Itdb_Playlist);
    gchar *new_name = StringValueCStr(name);
    gchar *old_name = playlist->name;

    /* Free up the old name. */
    g_free(old_name);

    /* Attach the new name. */
    playlist->name = new_name;

    return Qnil;
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

    /* If this is the master playlist, it always comes first. */
    if (rbpod_playlist_master_p(self) == Qtrue) {
        return NUM2INT(-1);
    }

    /* If we're comparing against the master playlist, it comes first. */
    if (rbpod_playlist_master_p(other) == Qtrue) {
        return NUM2INT(1);
    }

    this_playlist_name  = rbpod_playlist_name_get(self);
    other_playlist_name = rbpod_playlist_name_get(other);

    /* Otherwise, compare by playlist name. */
    return rb_str_cmp(this_playlist_name, other_playlist_name);
}

/*
 * call-seq:
 *     initialize() -> RbPod::Playlist
 *
 * Creates a detached playlist. (Not managed by the database.)
 */
static VALUE rbpod_playlist_initialize(VALUE self)
{
    /* Nothing to see here. */
    return self;
}

static void rbpod_playlist_deallocate(void *handle)
{
    Itdb_Playlist *playlist = (Itdb_Playlist *) handle;

    /* This playlist was unmanaged, so free it manually. */
    if (playlist->itdb == NULL) {
        itdb_playlist_free(playlist);
    }

    return;
}

static VALUE rbpod_playlist_allocate(VALUE self)
{
    Itdb_Playlist *playlist = itdb_playlist_new("New Playlist", FALSE);
    return Data_Wrap_Struct(cRbPodPlaylist, NULL, rbpod_playlist_deallocate, (void *) playlist);
}

void Init_rbpod_playlist(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    cRbPodPlaylist = rb_define_class_under(mRbPod, "Playlist", rb_cObject);

    rb_define_alloc_func(cRbPodPlaylist, rbpod_playlist_allocate);

    rb_define_method(cRbPodPlaylist, "initialize", rbpod_playlist_initialize, 0);

    rb_define_method(cRbPodPlaylist, "<=>", rbpod_playlist_compare, 1);

    rb_define_method(cRbPodPlaylist, "name", rbpod_playlist_name_get, 0);
    rb_define_method(cRbPodPlaylist, "length", rbpod_playlist_length_get, 0);
    rb_define_method(cRbPodPlaylist, "tracks", rbpod_playlist_tracks_get, 0);
    rb_define_method(cRbPodPlaylist, "created_on", rbpod_playlist_timestamp_get, 0);

    rb_define_method(cRbPodPlaylist, "name=", rbpod_playlist_name_set, 1);
    rb_define_method(cRbPodPlaylist, "smart=", rbpod_playlist_smart_set, 1);

    rb_define_method(cRbPodPlaylist, "smart?", rbpod_playlist_smart_p, 0);
    rb_define_method(cRbPodPlaylist, "master?", rbpod_playlist_master_p, 0);
    rb_define_method(cRbPodPlaylist, "podcast?", rbpod_playlist_podcast_p, 0);

    rb_define_method(cRbPodPlaylist, "shuffle!", rbpod_playlist_shuffle_bang, 0);

    return;
}

