/* playlist_collection.c */

#include "rbpod.h"

/*
 * :nodoc:
 */
static VALUE rbpod_playlist_collection_database(VALUE self)
{
    return rb_iv_get(self, "@parent");
}

/*
 * call-seq:
 *     size() -> Fixnum
 *
 * Returns the total number of playlists in this database.
 */
static VALUE rbpod_playlist_collection_size(VALUE self)
{
    VALUE database = rbpod_playlist_collection_database(self);
    Itdb_iTunesDB *_database = TYPED_DATA_PTR(database, Itdb_iTunesDB);
    return INT2NUM(itdb_playlists_number(_database));
}

/*
 * call-seq:
 *     master() -> RbPod::Playlist
 *
 * Returns the master playlist from the database.
 *
 */
static VALUE rbpod_playlist_collection_master(VALUE self)
{
    VALUE database = rbpod_playlist_collection_database(self);
    Itdb_iTunesDB *_database = TYPED_DATA_PTR(database, Itdb_iTunesDB);
    Itdb_Playlist *_playlist = itdb_playlist_mpl(_database);

    if (_playlist == NULL) {
        return Qnil;
    }

    /* Create a new instance of the master playlist from the data provided. */
    return rb_class_new_instance_with_data(0, NULL, cRbPodPlaylist, _playlist);
}

/*
 * call-seq:
 *     podcast() -> RbPod::Playlist
 *
 * Returns the podcast playlist from the database.
 */
static VALUE rbpod_playlist_collection_podcast(VALUE self)
{
    VALUE database = rbpod_playlist_collection_database(self);
    Itdb_iTunesDB *_database = TYPED_DATA_PTR(database, Itdb_iTunesDB);
    Itdb_Playlist *_playlist = itdb_playlist_podcasts(_database);

    if (_playlist == NULL) {
        return Qnil;
    }

    /* Create a new instance of the podcast playlist from the data provided. */
    return rb_class_new_instance_with_data(0, NULL, cRbPodPlaylist, _playlist);
}

/*
 * call-seq:
 *     include?(playlist) -> boolean
 *
 * Returns true or false if the given RbPod::Playlist +playlist+ exists within the database.
 */
static VALUE rbpod_playlist_collection_include_p(VALUE self, VALUE playlist)
{
    VALUE database = rbpod_playlist_collection_database(self);
    Itdb_iTunesDB *_database = TYPED_DATA_PTR(database, Itdb_iTunesDB);
    Itdb_Playlist *_playlist = TYPED_DATA_PTR(playlist, Itdb_Playlist);

    if (rb_obj_is_instance_of(playlist, cRbPodPlaylist) == FALSE) {
        rb_raise(eRbPodError, "Invalid argument: Expected RbPod::Playlist, got %s", StringValueCStr(playlist));
        return Qfalse;
    }

    return BooleanValue(itdb_playlist_exists(_database, _playlist));
}

/*
 * call-seq:
 *     insert(playlist, position = -1) -> nil
 *
 * Adds the given RbPod::Playlist +playlist+ to the database.
 */
static VALUE rbpod_playlist_collection_insert(int argc, VALUE *argv, VALUE self)
{
    VALUE database = rbpod_playlist_collection_database(self);
    VALUE playlist = Qnil, position = Qnil;
    Itdb_iTunesDB *_database = NULL;
    Itdb_Playlist *_playlist = NULL;
    gint32 _position = 0;

    if (rb_scan_args(argc, argv, "11", &playlist, &position) < 1) {
        rb_raise(eRbPodError, "Invalid arguments: Expected >= 1 argument!");
        return Qnil;
    }

    _database = TYPED_DATA_PTR(database, Itdb_iTunesDB);
    _playlist = TYPED_DATA_PTR(playlist, Itdb_Playlist);
    _position = NIL_P(position) ? -1 : NUM2INT(position);

    /* We we given a playlist? */
    if (rb_obj_is_instance_of(playlist, cRbPodPlaylist) == FALSE) {
        rb_raise(eRbPodError, "Invalid argument: expected RbPod::Playlist, got %s", StringValueCStr(playlist));
        return Qnil;
    }

    /* Does this playlist already exist in the database? */
    if (itdb_playlist_exists(_database, _playlist) == TRUE) {
        rb_raise(eRbPodError, "Invalid argument: this playlist already exists in the database!");
        return Qnil;
    }

    itdb_playlist_add(_database, _playlist, _position);

    return Qnil;
}

/*
 * call-seq:
 *     remove(playlist) -> nil
 *
 * Removes the given RbPod::Playlist +playlist+ from the database.
 */
static VALUE rbpod_playlist_collection_remove(VALUE self, VALUE playlist)
{
    VALUE database = rbpod_playlist_collection_database(self);
    Itdb_iTunesDB *_database = TYPED_DATA_PTR(database, Itdb_iTunesDB);
    Itdb_Playlist *_playlist = TYPED_DATA_PTR(playlist, Itdb_Playlist);

    /* We we given a playlist? */
    if (rb_obj_is_instance_of(playlist, cRbPodPlaylist) == FALSE) {
        rb_raise(eRbPodError, "Invalid argument: Expected RbPod::Playlist, got %s", StringValueCStr(playlist));
        return Qnil;
    }

    /* Does this playlist already exist in the database? */
    if (itdb_playlist_exists(_database, _playlist) == FALSE) {
        rb_raise(eRbPodError, "Invalid argument: This playlist does not exist in the database!");
        return Qnil;
    }

    itdb_playlist_remove(_playlist);

    return Qnil;
}

void Init_rbpod_playlist_collection(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    mRbPodPlaylistCollection = rb_define_module_under(mRbPod, "PlaylistCollection");

    rb_define_method(mRbPodPlaylistCollection, "size", rbpod_playlist_collection_size, 0);

    rb_define_method(mRbPodPlaylistCollection, "master", rbpod_playlist_collection_master, 0);
    rb_define_method(mRbPodPlaylistCollection, "podcast", rbpod_playlist_collection_podcast, 0);

    rb_define_method(mRbPodPlaylistCollection, "include?", rbpod_playlist_collection_include_p, 1);

    rb_define_method(mRbPodPlaylistCollection, "insert", rbpod_playlist_collection_insert, -1);
    rb_define_method(mRbPodPlaylistCollection, "remove", rbpod_playlist_collection_remove, 1);

    /* Syntactic sugar for adding a playlist. */
    rb_define_alias(mRbPodPlaylistCollection, "<<", "insert");

    return;
}

