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
    return rb_ivar_get(self, rb_intern("@database"));
}

/*
 * call-seq:
 *     initialize(database) -> RbPod::PlaylistCollection
 *
 * Given an RbPod::Database +database+ creates a collection of playlists from it.
 */
static VALUE rbpod_playlist_collection_initialize(VALUE self, VALUE database)
{
    rb_ivar_set(self, rb_intern("@database"), database);
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

    return;
}

