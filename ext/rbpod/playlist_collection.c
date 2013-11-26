/* playlist_collection.c */

#include "rbpod.h"
#include "playlist.h"
#include "collection.h"
#include "playlist_collection.h"

/*
 * call-seq:
 *     database() -> RbPod::Database
 *
 * Returns the database this playlist collection is attached to.
 */
static VALUE rbpod_playlist_collection_database(VALUE self)
{
    return rb_iv_get(self, "@database");
}

/*
 * call-seq:
 *     master() -> RbPod::Playlist
 *
 * Returns the master playlist from the database.
 */
static VALUE rbpod_playlist_collection_master(VALUE self)
{
    VALUE master_playlist_name, database = rbpod_playlist_collection_database(self);
    Itdb_iTunesDB *_database = TYPED_DATA_PTR(database, Itdb_iTunesDB);
    Itdb_Playlist *_playlist = itdb_playlist_mpl(_database);

    /* Extract the master playlist name from the playlist. */
    master_playlist_name = rb_str_new2(_playlist->name);

    /* Create a new instance of the master playlist from the data provided. */
    return rb_class_new_instance_with_data(1, &master_playlist_name, cRbPodPlaylist, _playlist);
}

/*
 * call-seq:
 *     podcast() -> RbPod::Playlist
 *
 * Returns the podcast playlist from the database.
 */
static VALUE rbpod_playlist_collection_podcast(VALUE self)
{
    VALUE podcast_playlist_name, database = rbpod_playlist_collection_database(self);
    Itdb_iTunesDB *_database = TYPED_DATA_PTR(database, Itdb_iTunesDB);
    Itdb_Playlist *_playlist = itdb_playlist_podcasts(_database);

    /* If we don't have a podcast playlist, make one. */
    if (_playlist == NULL) {
        return Qnil;
    }

    /* Extract the podcast playlist name from the playlist. */
    podcast_playlist_name = rb_str_new2(_playlist->name);

    /* Create a new instance of the podcast playlist from the data provided. */
    return rb_class_new_instance_with_data(1, &podcast_playlist_name, cRbPodPlaylist, _playlist);
}

/*
 * call-seq:
 *     initialize(database) -> RbPod::PlaylistCollection
 *
 * Given an RbPod::Database +database+ creates a collection of playlists from it.
 */
static VALUE rbpod_playlist_collection_initialize(VALUE self, VALUE database)
{
    rb_iv_set(self, "@database", database);
    return self;
}

static VALUE rbpod_playlist_collection_type(VALUE self)
{
    return cRbPodPlaylist;
}

void Init_rbpod_playlist_collection(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    cRbPodPlaylistCollection = rb_define_class_under(mRbPod, "PlaylistCollection", rb_cObject);

    rb_include_module(cRbPodPlaylistCollection, mRbPodCollection);

    rb_define_private_method(cRbPodPlaylistCollection, "type", rbpod_playlist_collection_type, 0);

    rb_define_method(cRbPodPlaylistCollection, "initialize", rbpod_playlist_collection_initialize, 1);

    rb_define_method(cRbPodPlaylistCollection, "database", rbpod_playlist_collection_database, 0);

    rb_define_method(cRbPodPlaylistCollection, "master", rbpod_playlist_collection_master, 0);

    rb_define_method(cRbPodPlaylistCollection, "podcast", rbpod_playlist_collection_podcast, 0);

    return;
}

