/* rbpod.h */

#ifndef RBPOD_H
#define RBPOD_H

#include <ruby.h>
#include <gpod/itdb.h>

#define BooleanValue(value) (value) ? Qtrue : Qfalse

#define TYPED_DATA_PTR(self, type) ((type *) DATA_PTR(self))

extern VALUE mRbPod;
extern VALUE eRbPodError;

#endif /* RBPOD_H */
