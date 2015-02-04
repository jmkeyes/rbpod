/* utilities.c */

#include "rbpod.h"

/*
 * Returns an enumerator with it's singleton class customized according to module.
 */
inline VALUE rb_custom_enumeratorize(VALUE object, VALUE argc, VALUE *argv, VALUE module)
{
    VALUE function   = ID2SYM(rb_frame_this_func());
    VALUE enumerator = rb_enumeratorize(object, function, argc, argv);

    /* XXX: This is evil. Don't do this. */
    rb_iv_set(enumerator, "@parent", object);

    /* Add our customizations to this enumerator. */
    rb_extend_object(enumerator, module);

    return enumerator;
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
    /* Return the instance. */
    return instance;
}
