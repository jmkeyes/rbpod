/* track_collection.c */

#include "rbpod.h"

/*
 * call-seq:
 *     playlist() -> RbPod::Playlist
 *
 * Returns the playlist this track collection is attached to.
 */
static VALUE rbpod_track_collection_playlist(VALUE self)
{
    VALUE playlist = rb_iv_get(self, "@playlist");

    if (rb_obj_is_instance_of(playlist, cRbPodPlaylist) == FALSE) {
        rb_raise(eRbPodError, "Invalid Arguments: Expected RbPod::Playlist, got %s", StringValueCStr(playlist));
        return Qnil;
    }

    return playlist;
}

/*
 * call-seq:
 *     include?(track) -> boolean
 *
 * Determines if the given RbPod::Track +track+ is within this playlist.
 */
static VALUE rbpod_track_collection_include(VALUE self, VALUE track)
{
    VALUE playlist = rbpod_track_collection_playlist(self);
    Itdb_Playlist *_playlist = TYPED_DATA_PTR(playlist, Itdb_Playlist);
    Itdb_Track *_track = TYPED_DATA_PTR(track, Itdb_Track);

    if (rb_obj_is_instance_of(track, cRbPodTrack) == FALSE) {
        rb_raise(eRbPodError, "Invalid Arguments: Expected RbPod::Track, got %s", StringValueCStr(playlist));
        return Qnil;
    }

    return BooleanValue(itdb_playlist_contains_track(_playlist, _track));
}

/*
 * call-seq:
 *     remove(track) -> nil
 *
 * Removes a given RbPod::Track +track+ from the playlist.
 */
static VALUE rbpod_track_collection_remove(VALUE self, VALUE track)
{
    VALUE playlist = rbpod_track_collection_playlist(self);
    Itdb_Playlist *_playlist = TYPED_DATA_PTR(playlist, Itdb_Playlist);
    Itdb_Track *_track = TYPED_DATA_PTR(track, Itdb_Track);

    if (rb_obj_is_instance_of(track, cRbPodTrack) == FALSE) {
        rb_raise(eRbPodError, "Invalid Arguments: Expected RbPod::Track, got %s", StringValueCStr(playlist));
        return Qnil;
    }

    itdb_playlist_remove_track(_playlist, _track);

    return Qnil;
}

/*
 * call-seq:
 *     insert(track, position = -1) -> nil
 *
 * Adds a given RbPod::Track +track+ to the playlist at +position+. If +position+
 * was not supplied, +track+ will be appended to the end of the playlist.
 */
static VALUE rbpod_track_collection_insert(int argc, VALUE *argv, VALUE self)
{
    VALUE playlist = rbpod_track_collection_playlist(self);
    Itdb_Playlist *_playlist = TYPED_DATA_PTR(playlist, Itdb_Playlist);
    VALUE track = Qnil, position = Qnil;
    Itdb_Track *_track = NULL;
    gint32 _position = 0;
    GError *error = NULL;

    if (rb_scan_args(argc, argv, "11", &track, &position) < 1) {
        rb_raise(eRbPodError, "Invalid arguments: Expected >= 1 argument!");
        return Qnil;
    }

    _track    = TYPED_DATA_PTR(track, Itdb_Track);
    _position = NIL_P(position) ? -1 : NUM2INT(position);

    /* We we given a track? */
    if (rb_obj_is_instance_of(track, cRbPodTrack) == FALSE) {
        rb_raise(eRbPodError, "Invalid argument: expected RbPod::Track, got %s", StringValueCStr(playlist));
        return Qnil;
    }

    /* Has this track already been transferred? If so, just add it to the playlist and be done. */
    if (_track->transferred == TRUE) {
        itdb_playlist_add_track(_playlist, _track, _position);
        return Qnil;
    }

    /* Do we have a source path for this track? */
    if (_track->userdata != NULL && g_file_test(_track->userdata, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_SYMLINK) == FALSE) {
        rb_raise(eRbPodError, "Invalid argument: Cannot add a track without a source file.");
        return Qnil;
    }

    itdb_playlist_add_track(_playlist, _track, _position);

    /* Copy the track to the iPod. XXX: Unlock the GVL instead, stupid. */
    if (itdb_cp_track_to_ipod(_track, _track->userdata, &error) == FALSE) {
        itdb_playlist_remove_track(_playlist, _track);
        return rbpod_raise_error(error);
    }


    return Qnil;
}

/*
 * call-seq:
 *     size() -> integer
 *
 * Returns the total number of tracks in this playlist.
 */
static VALUE rbpod_track_collection_size(VALUE self)
{
    VALUE playlist = rbpod_track_collection_playlist(self);
    Itdb_Playlist *_playlist = TYPED_DATA_PTR(playlist, Itdb_Playlist);
    return INT2NUM(itdb_playlist_tracks_number(_playlist));
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

/*
 * :nodoc:
 */
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

    rb_define_method(cRbPodTrackCollection, "size", rbpod_track_collection_size, 0);

    rb_define_method(cRbPodTrackCollection, "insert", rbpod_track_collection_insert, -1);

    rb_define_method(cRbPodTrackCollection, "remove", rbpod_track_collection_remove, 1);

    rb_define_method(cRbPodTrackCollection, "include?", rbpod_track_collection_include, 1);

    /* Syntactic sugar for adding a track. */
    rb_define_alias(cRbPodTrackCollection, "<<", "insert");

    return;
}

