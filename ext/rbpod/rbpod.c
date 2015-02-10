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

/*
 * :nodoc:
 */
static void rbpod_glib_log_handler(const gchar *domain, GLogLevelFlags level, const gchar *message, gpointer userdata)
{
    rb_warn("DOMAIN: '%s' LEVEL: %08x MESSAGE: '%s' DATA: %p", domain, level, message, userdata);
    return;
}

void Init_rbpod(void)
{
    /* Setup a GLib log handler for all warning messages. */
    g_log_set_default_handler(&rbpod_glib_log_handler, NULL);

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


