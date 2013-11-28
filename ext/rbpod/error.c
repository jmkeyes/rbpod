/* error.c */

#include "rbpod.h"

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

void Init_rbpod_error(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    /* This is a generic subclass of RuntimeError for RbPod-specific errors.*/
    eRbPodError = rb_define_class_under(mRbPod, "Error", rb_eRuntimeError);
}

