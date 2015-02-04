/* rbpod.c */

#include "rbpod.h"

VALUE mRbPod;
VALUE eRbPodError;

VALUE cRbPodDatabase;
VALUE cRbPodDevice;

VALUE cRbPodTrack;
VALUE cRbPodPlaylist;

VALUE mRbPodTrackCollection;
VALUE mRbPodPlaylistCollection;

VALUE rb_cPathname;

/*
 * call-seq:
 *     RbPod(mount_point) -> RbPod::Database
 *
 * A shortcut for creating an RbPod::Database instance from a mount point.
 *
 */
static VALUE rbpod_load_database(VALUE self, VALUE mount_point)
{
    return rb_class_new_instance(1, &mount_point, cRbPodDatabase);
}

void Init_rbpod(void)
{
    /* This is the wrapper for all RbPod related classes. */
    mRbPod = rb_define_module("RbPod");

    rb_require("pathname");

    Init_rbpod_error();
    Init_rbpod_database();
    Init_rbpod_device();
    Init_rbpod_playlist();
    Init_rbpod_track();
    Init_rbpod_track_collection();
    Init_rbpod_playlist_collection();

    rb_define_global_function("RbPod", rbpod_load_database, 1);

    rb_cPathname = rb_const_get(rb_cObject, rb_intern("Pathname"));

    return;
}


