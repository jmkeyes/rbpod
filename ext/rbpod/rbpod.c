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

void Init_rbpod(void)
{
    /* Set up the top-level RbPod module. */
    mRbPod = rb_define_module("RbPod");

    /* Subclass RuntimeError for ourselves. */
    eRbPodError = rb_define_class_under(mRbPod, "Error", rb_eRuntimeError);

    /* Set up the RbPod::Database class. */
    Init_rbpod_database();

    /* Set up the RbPod::Device class. */
    Init_rbpod_device();

    /* Set up the RbPod::Collection class. */
    Init_rbpod_collection();

    /* Set up the RbPod::Playlist class. */
    Init_rbpod_playlist();
    Init_rbpod_playlist_collection();

    /* Set up the RbPod::Track class. */
    Init_rbpod_track();
    Init_rbpod_track_collection();

    return;
}


