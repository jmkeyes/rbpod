/* rbpod.c */

#include "rbpod.h"
#include "track.h"
#include "device.h"
#include "playlist.h"
#include "database.h"
#include "collection.h"
#include "track_collection.h"
#include "playlist_collection.h"

VALUE mRbPod;
VALUE eRbPodError;

VALUE cRbPodDatabase;
VALUE cRbPodDevice;

VALUE cRbPodCollection;

VALUE cRbPodPlaylist;
VALUE mRbPodPlaylistCollection;

VALUE cRbPodTrack;
VALUE mRbPodTrackCollection;

inline VALUE rbpod_raise_error(GError *error)
{
    VALUE error_message;

    if (error != NULL) {
        error_message = rb_str_new2(error->message);
        g_error_free(error);
        rb_raise(eRbPodError, "%s", StringValueCStr(error_message));
    }

    return Qnil;
}

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

    /* This is a generic subclass of RuntimeError for RbPod-specific errors.*/
    eRbPodError = rb_define_class_under(mRbPod, "Error", rb_eRuntimeError);

    Init_rbpod_database();
    Init_rbpod_device();
    Init_rbpod_collection();
    Init_rbpod_playlist();
    Init_rbpod_playlist_collection();
    Init_rbpod_track();
    Init_rbpod_track_collection();

    rb_define_global_function("RbPod", rbpod_load_database, 1);

    return;
}


