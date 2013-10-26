/* rbpod.c */

#include "rbpod.h"
#include "track.h"
#include "device.h"
#include "playlist.h"
#include "database.h"
#include "collection.h"

VALUE mRbPod;
VALUE eRbPodError;

void Init_rbpod(void) {
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

    /* Set up the RbPod::Track class. */
    Init_rbpod_track();

    return;
}


