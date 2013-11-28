/* rbpod.c */

#include "rbpod.h"

VALUE mRbPod;
VALUE eRbPodError;

VALUE cRbPodDatabase;
VALUE cRbPodDevice;

VALUE cRbPodTrack;
VALUE cRbPodPlaylist;

VALUE mRbPodCollection;

VALUE cRbPodTrackCollection;
VALUE cRbPodPlaylistCollection;

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
 * This is a hack used to inject a pointer from the data of one class instance into another.
 */
inline VALUE rb_class_new_instance_with_data(int argc, VALUE *argv, VALUE class, void *handle)
{
    /* Create a new instance of this class. */
    VALUE instance = rb_class_new_instance(argc, argv, class);

    /* Assign it's DATA pointer to the given handle. */
    DATA_PTR(instance) = handle;

    return instance;
}

/*
 * This is a hack so that including a module will trigger the +included+ singleton method.
 */
inline void rb_real_include_module(VALUE klass, VALUE module)
{
    ID included = rb_intern("included");

    rb_include_module(klass, module);

    rb_funcall(mRbPodCollection, included, 1, klass);
}

void Init_rbpod(void)
{
    /* This is the wrapper for all RbPod related classes. */
    mRbPod = rb_define_module("RbPod");

    Init_rbpod_error();
    Init_rbpod_database();
    Init_rbpod_device();
    Init_rbpod_collection();
    Init_rbpod_playlist();
    Init_rbpod_playlist_collection();
    Init_rbpod_track();
    Init_rbpod_track_collection();

    rb_define_global_function("RbPod", rbpod_load_database, 1);

    rb_cPathname = rb_const_get(rb_cObject, rb_intern("Pathname"));

    return;
}


