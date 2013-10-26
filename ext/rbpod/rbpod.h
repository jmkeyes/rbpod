/* rbpod.h */

#ifndef RBPOD_H
#define RBPOD_H

#include <ruby.h>
#include <gpod/itdb.h>

#define BooleanValue(value) (value) ? Qtrue : Qfalse

extern VALUE mRbPod;
extern VALUE eRbPodError;

#endif /* RBPOD_H */
