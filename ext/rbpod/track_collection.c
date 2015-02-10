/* track_collection.c */

#include "rbpod.h"

/*
 * :nodoc:
 */
static VALUE rbpod_track_collection_playlist(VALUE self)
{
    return rb_iv_get(self, "@parent");
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
 *     include?(track) -> boolean
 *
 * Determines if the given RbPod::Track +track+ is within this playlist.
 */
static VALUE rbpod_track_collection_include_p(VALUE self, VALUE track)
{
    VALUE playlist = rbpod_track_collection_playlist(self);
    Itdb_Playlist *_playlist = TYPED_DATA_PTR(playlist, Itdb_Playlist);
    Itdb_Track *_track = TYPED_DATA_PTR(track, Itdb_Track);

    if (rb_obj_is_instance_of(track, cRbPodTrack) == FALSE) {
        rb_raise(eRbPodError, "Invalid Arguments: Expected RbPod::Track, got %s", StringValueCStr(track));
        return Qnil;
    }

    return BooleanValue(itdb_playlist_contains_track(_playlist, _track));
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
    Itdb_iTunesDB *_database = _playlist->itdb;
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
        rb_raise(eRbPodError, "Invalid argument: expected RbPod::Track, got %s", StringValueCStr(track));
        return Qnil;
    }

    /* Do we have a source path for this track? */
    if (_track->userdata != NULL && g_file_test(_track->userdata, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_SYMLINK) == FALSE) {
        rb_raise(eRbPodError, "Invalid argument: Cannot add a track without a source file.");
        return Qnil;
    }

    /* If the master database does not contain this track, add it to the database. */
    if (itdb_playlist_contains_track(itdb_playlist_mpl(_database), _track) == FALSE) {
        itdb_track_add(_database, _track, -1);
    }

    /* Add this track to the playlist. */
    itdb_playlist_add_track(_playlist, _track, _position);

    /* Copy the track to the iPod. XXX: Unlock the GVL instead, stupid. */
    if (itdb_cp_track_to_ipod(_track, _track->userdata, &error) == FALSE) {
        return rbpod_raise_error(error);
    }

    return Qnil;
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

void Init_rbpod_track_collection(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    mRbPodTrackCollection = rb_define_module_under(mRbPod, "TrackCollection");

    rb_define_method(mRbPodTrackCollection, "size", rbpod_track_collection_size, 0);

    rb_define_method(mRbPodTrackCollection, "include?", rbpod_track_collection_include_p, 1);

    rb_define_method(mRbPodTrackCollection, "insert", rbpod_track_collection_insert, -1);
    rb_define_method(mRbPodTrackCollection, "remove", rbpod_track_collection_remove, 1);

    /* Syntactic sugar for adding a track. */
    rb_define_alias(mRbPodTrackCollection, "<<", "insert");

    return;
}

