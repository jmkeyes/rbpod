/* rbpod.h */

#ifndef RBPOD_H
#define RBPOD_H

#include <ruby.h>

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#ifdef HAVE_GPOD_ITDB_H
#include <gpod/itdb.h>
#endif

/* C pre-processor macro defintions. */
#define BooleanValue(value) (value) ? Qtrue : Qfalse

#define TYPED_DATA_PTR(self, type) ((type *) DATA_PTR(self))

#ifdef HAVE_STDDEF_H
#define MEMBER_OF(structure, member)     ((structure *)0)->member
#define MEMBER_SIZE(structure, member)   sizeof(MEMBER_OF(structure, member))
#define MEMBER_OFFSET(structure, member) offsetof(MEMBER_OF(structure, member))
#endif

/* Initialization function prototypes. */
RUBY_EXTERN void Init_rbpod(void);                       /* rbpod.c */
RUBY_EXTERN void Init_rbpod_error(void);                 /* error.c */
RUBY_EXTERN void Init_rbpod_database(void);              /* database.c */
RUBY_EXTERN void Init_rbpod_device(void);                /* device.c */
RUBY_EXTERN void Init_rbpod_track(void);                 /* track.c */
RUBY_EXTERN void Init_rbpod_playlist(void);              /* playlist.c */
RUBY_EXTERN void Init_rbpod_collection(void);            /* collection.c */
RUBY_EXTERN void Init_rbpod_track_collection(void);      /* track_collection.c */
RUBY_EXTERN void Init_rbpod_playlist_collection(void);   /* playlist_collection.c */

/* Helper function prototypes. */
RUBY_EXTERN inline VALUE rbpod_raise_error(GError *error);
RUBY_EXTERN inline VALUE rb_class_new_instance_with_data(int argc, VALUE *argv, VALUE class, void *handle);
RUBY_EXTERN inline void rb_real_include_module(VALUE klass, VALUE module);

/* Global variables. */
RUBY_EXTERN VALUE mRbPod;                   /* rbpod.c */
RUBY_EXTERN VALUE eRbPodError;              /* error.c */
RUBY_EXTERN VALUE cRbPodDatabase;           /* database.c */
RUBY_EXTERN VALUE cRbPodDevice;             /* device.c */
RUBY_EXTERN VALUE cRbPodTrack;              /* track.c */
RUBY_EXTERN VALUE cRbPodPlaylist;           /* playlist.c */
RUBY_EXTERN VALUE mRbPodCollection;         /* collection.c */
RUBY_EXTERN VALUE cRbPodTrackCollection;    /* track_collection.c */
RUBY_EXTERN VALUE cRbPodPlaylistCollection; /* playlist_collection.c */

RUBY_EXTERN VALUE rb_cPathname;

#endif /* RBPOD_H */
