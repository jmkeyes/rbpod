/* rbpod.h */

#ifndef RBPOD_H
#define RBPOD_H

#include <ruby.h>
#include <gpod/itdb.h>

#define BooleanValue(value) (value) ? Qtrue : Qfalse

#define TYPED_DATA_PTR(self, type) ((type *) DATA_PTR(self))

RUBY_EXTERN VALUE mRbPod;
RUBY_EXTERN VALUE eRbPodError;

RUBY_EXTERN VALUE rb_cPathname;

inline VALUE rbpod_raise_error(GError *error);

#endif /* RBPOD_H */
