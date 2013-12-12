/* utilities.c */

#include "rbpod.h"

/*
 * This is a hack used to inject a pointer from the data of one class instance into another.
 */
inline VALUE rb_class_new_instance_with_data(int argc, VALUE *argv, VALUE class, void *handle)
{
    /* Create a new instance of this class. */
    VALUE instance = rb_class_new_instance(argc, argv, class);
    /* Assign it's DATA pointer to the given handle. */
    DATA_PTR(instance) = handle;
    /* Return the instance. */
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

