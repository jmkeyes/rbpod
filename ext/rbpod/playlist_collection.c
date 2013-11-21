/* playlist_collection.c */

#include "rbpod.h"
#include "playlist.h"
#include "collection.h"
#include "playlist_collection.h"

/*
 * call-seq:
 *     database() -> RbPod::Database
 *
 * The database from which this collection of playlists is attached to.
 */
static VALUE rbpod_playlist_collection_database(VALUE self)
{
    return rb_iv_get(self, "@database");
}

/*
 * call-seq:
 *     [](index) -> RbPod::Playlist
 *     [](name) -> RbPod::Playlist
 *
 * Returns a playlist by either position or name in the collection, or nil if it wasn't found.
 */
static VALUE rbpod_playlist_collection_get(VALUE self, VALUE key)
{
    VALUE parent = rbpod_playlist_collection_database(self);
    Itdb_iTunesDB *database = TYPED_DATA_PTR(parent, Itdb_iTunesDB);
    Itdb_Playlist *playlist = NULL;
    int key_type = TYPE(key);

    if (key_type == T_SYMBOL || key_type == T_STRING) {
        playlist = itdb_playlist_by_name(database, StringValueCStr(key));
        return Data_Wrap_Struct(cRbPodPlaylist, NULL, NULL, (void *) playlist);
    }

    if (key_type == T_FIXNUM) {
        playlist = itdb_playlist_by_nr(database, FIX2INT(key));
        return Data_Wrap_Struct(cRbPodPlaylist, NULL, NULL, (void *) playlist);
    }

    return Qnil;
}

/*
 * call-seq:
 *     podcast() -> RbPod::Playlist
 *
 * Returns the podcast playlist in this collection.
 */
static VALUE rbpod_playlist_collection_podcast_get(VALUE self)
{
    VALUE parent = rbpod_playlist_collection_database(self);
    Itdb_iTunesDB *database = TYPED_DATA_PTR(parent, Itdb_iTunesDB);
    Itdb_Playlist *podcasts = itdb_playlist_podcasts(database);
    return Data_Wrap_Struct(cRbPodPlaylist, NULL, NULL, (void *) podcasts);
}

/*
 * call-seq:
 *     master() -> RbPod::Playlist
 *
 * Returns the master playlist in this collection.
 */
static VALUE rbpod_playlist_collection_master_get(VALUE self)
{
    VALUE parent = rbpod_playlist_collection_database(self);
    Itdb_iTunesDB *database = TYPED_DATA_PTR(parent, Itdb_iTunesDB);
    Itdb_Playlist *master = itdb_playlist_mpl(database);
    return Data_Wrap_Struct(cRbPodPlaylist, NULL, NULL, (void *) master);
}

inline VALUE rbpod_playlist_collection_create(VALUE database, GList *items)
{
    VALUE collection = rbpod_collection_create(items, cRbPodPlaylist);
    rb_extend_object(collection, mRbPodPlaylistCollection);
    rb_iv_set(collection, "@database", database);
    return collection;
}

void Init_rbpod_playlist_collection(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    /* This module extends any collection of playlists at runtime. */
    mRbPodPlaylistCollection = rb_define_module_under(mRbPod, "PlaylistCollection");

    rb_define_method(mRbPodPlaylistCollection, "database", rbpod_playlist_collection_database, 0);

    rb_define_method(mRbPodPlaylistCollection, "master", rbpod_playlist_collection_master_get, 0);
    rb_define_method(mRbPodPlaylistCollection, "podcast", rbpod_playlist_collection_podcast_get, 0);

    rb_define_method(mRbPodPlaylistCollection, "[]", rbpod_playlist_collection_get, 1);

    return;
}

