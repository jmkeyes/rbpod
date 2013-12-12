/* track.c */

#include "rbpod.h"

/*
 * call-seq:
 *     transferred?() -> Boolean
 *
 * Returns true or false if this track has been transferred to the device.
 */
static VALUE rbpod_track_transferred_p(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return BooleanValue(track->transferred);
}

/*
 * call-seq:
 *     path() -> String
 *
 * Returns the full path to the file backing this track on the device.
 */
static VALUE rbpod_track_path_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    gchar *buffer = NULL;
    VALUE path;

    /* If there's no database or file path, this track hasn't been transferred. */
    if (track->itdb == NULL || track->ipod_path == NULL) {
        return Qnil;
    }

    /* Find the full path to the track. */
    buffer = itdb_filename_on_ipod(track);

    /* Conert it to a string. */
    path = rb_str_new2(buffer);

    /* Free the buffer. */
    g_free(buffer);

    /* Return a Pathname instance for the resolved path. */
    return rb_class_new_instance(1, &path, rb_cPathname);
}

/*
 * call-seq:
 *     type() -> String
 *
 * Returns a human-readable string describing the content of the track.
 */
static VALUE rbpod_track_type_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return rb_str_new2(track->filetype);
}

/*
 * call-seq:
 *     artist() -> String
 *
 * Returns the artist for the track.
 */
static VALUE rbpod_track_artist_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return rb_str_new2(track->artist);
}

/*
 * call-seq:
 *     album() -> String
 *
 * Returns the album for the track.
 */
static VALUE rbpod_track_album_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return rb_str_new2(track->album);
}

/*
 * call-seq:
 *     title() -> String
 *
 * Returns the title of the track.
 */
static VALUE rbpod_track_title_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return rb_str_new2(track->title);
}

/*
 * call-seq:
 *     id() -> Fixnum
 *
 * Returns the internal ID number for this track. Do not use this method.
 */
static VALUE rbpod_track_id_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return INT2NUM(track->id);
}

/*
 * call-seq:
 *     initialize() -> RbPod::Track
 *
 * Creates a detached track. (Not managed by a database.)
 */
static VALUE rbpod_track_initialize(VALUE self)
{
    return self;
}

static void rbpod_track_deallocate(void *handle)
{
    Itdb_Track *track = (Itdb_Track *) handle;

    /* Don't free tracks that are assigned to a playlist/database. */
    if (track->itdb == NULL || track->id == 0) {
        itdb_track_free((Itdb_Track *) handle);
    }

    return;
}

static VALUE rbpod_track_allocate(VALUE self)
{
    Itdb_Track *track = itdb_track_new();
    return Data_Wrap_Struct(cRbPodTrack, NULL, rbpod_track_deallocate, (void *) track);
}

void Init_rbpod_track(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    cRbPodTrack = rb_define_class_under(mRbPod, "Track", rb_cObject);

    rb_define_alloc_func(cRbPodTrack, rbpod_track_allocate);

    rb_define_method(cRbPodTrack, "initialize", rbpod_track_initialize, 0);

    rb_define_private_method(cRbPodTrack, "id", rbpod_track_id_get, 0);

    rb_define_method(cRbPodTrack, "title", rbpod_track_title_get, 0);
    rb_define_method(cRbPodTrack, "album", rbpod_track_album_get, 0);
    rb_define_method(cRbPodTrack, "artist", rbpod_track_artist_get, 0);
    rb_define_method(cRbPodTrack, "type", rbpod_track_type_get, 0);
    rb_define_method(cRbPodTrack, "path", rbpod_track_path_get, 0);

    rb_define_method(cRbPodTrack, "transferred?", rbpod_track_transferred_p, 0);

    return;
}

