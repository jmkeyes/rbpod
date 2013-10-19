/* database.c */

#include "rbpod.h"
#include "track.h"
#include "device.h"
#include "playlist.h"
#include "database.h"
#include "collection.h"

/*
 * Although libgpod has an itdb_new(), itdb_parse() also returns a pointer to a
 * newly allocated Itdb_iTunesDB structure. Since overwriting the data pointer from
 * Data_Get_Struct() at runtime is dangerous, we use a double-pointer to wrap the
 * Itdb_iTunesDB handle, so as not to confuse the Ruby implementation internals.
 */

VALUE cRbPodDatabase;

static inline Itdb_iTunesDB *rbpod_database_unwrap(VALUE self) {
    Itdb_iTunesDB **database = NULL;
    Data_Get_Struct(self, Itdb_iTunesDB *, database);
    return *database;
}

static VALUE rbpod_database_write(VALUE self) {
    Itdb_iTunesDB *database = rbpod_database_unwrap(self);
    gboolean success = itdb_write(database, NULL); /* TODO: Improve error handling. */
    return BooleanValue(success);
}

static VALUE rbpod_database_is_synchronized(VALUE self) {
    Itdb_iTunesDB *database = rbpod_database_unwrap(self);
    guint32 nontransferred = itdb_tracks_number_nontransferred(database);
    return BooleanValue(nontransferred == 0);
}

static VALUE rbpod_database_playlists_get(VALUE self) {
    Itdb_iTunesDB *database = rbpod_database_unwrap(self);
    return rbpod_collection_wrap(database->playlists, cRbPodPlaylist, FALSE);
}

static VALUE rbpod_database_tracks_get(VALUE self) {
    Itdb_iTunesDB *database = rbpod_database_unwrap(self);
    return rbpod_collection_wrap(database->tracks, cRbPodTrack, FALSE);
}

static VALUE rbpod_database_device_get(VALUE self) {
    Itdb_iTunesDB *database = rbpod_database_unwrap(self);
    return rbpod_device_wrap(database->device, FALSE);
}

static VALUE rbpod_database_filename_get(VALUE self) {
    Itdb_iTunesDB *database = rbpod_database_unwrap(self);
    return rb_str_new2(database->filename);
}

static VALUE rbpod_database_version_get(VALUE self) {
    Itdb_iTunesDB *database = rbpod_database_unwrap(self);
    return INT2NUM(database->version);
}

static VALUE rbpod_database_id_get(VALUE self) {
    Itdb_iTunesDB *database = rbpod_database_unwrap(self);
    return INT2NUM(database->id);
}

static VALUE rbpod_database_mountpoint_get(VALUE self) {
    Itdb_iTunesDB *database = rbpod_database_unwrap(self);
    return rb_str_new2(itdb_get_mountpoint(database));
}

static VALUE rbpod_database_mountpoint_set(VALUE self, VALUE path) {
    Itdb_iTunesDB *database = rbpod_database_unwrap(self);
    itdb_set_mountpoint(database, StringValueCStr(path));
    return rbpod_database_mountpoint_get(self);
}

static void rbpod_database_deallocate(void *handle) {
    Itdb_iTunesDB **database = (Itdb_iTunesDB **) handle;
    itdb_free(*database);
    xfree(handle);
    return;
}

static VALUE rbpod_database_create(VALUE self, VALUE device_name, VALUE mount_point, VALUE model_number) {
    Itdb_iTunesDB **database = ALLOC(Itdb_iTunesDB *);
    gchar *_mount_point, *_model_number, *_device_name;

    _device_name  = StringValueCStr(device_name);
    _mount_point  = StringValueCStr(mount_point);

    /* GPod can function with a NULL model number, however, artwork will not function. */
    _model_number = !NIL_P(model_number) ? StringValueCStr(model_number) : NULL;

    /* Initialize the iPod at this mount point, with this device name and model number. */
    /* TODO: Improve error handling. */
    if (itdb_init_ipod(_mount_point, _model_number, _device_name, NULL) == FALSE) {
        free(database);
        rb_raise(rb_eRuntimeError, "Unable to format this iPod.");
        return Qfalse;
    }

    /* Parse the newly created database. */
    /* TODO: Improve error handling. */
    *database = itdb_parse(_mount_point, NULL);

    if (*database == NULL) {
        free(database);
        rb_raise(rb_eRuntimeError, "Not an iPod mount point.");
        return Qnil;
    }

    /* Return an instance of this class using the newly created database. */
    return Data_Wrap_Struct(self, NULL, rbpod_database_deallocate, (void *) database);
}

static VALUE rbpod_database_initialize(VALUE self, VALUE mount_point) {
    Itdb_iTunesDB **database;

    /* Extract the wrapper from the class itself. */
    Data_Get_Struct(self, Itdb_iTunesDB *, database);

    /* Try to parse the database from the given mount point. */
    /* TODO: Improve error handling. */
    *database = itdb_parse(StringValueCStr(mount_point), NULL);

    /* The given mount point was not an iPod mount point. */
    if (*database == NULL) {
        rb_raise(rb_eRuntimeError, "Not an iPod mount point.");
        return Qnil;
    }

    return self;
}

inline VALUE rbpod_database_wrap(Itdb_iTunesDB **database, gboolean freeable) {
    return Data_Wrap_Struct(cRbPodDatabase, NULL, freeable ? rbpod_database_deallocate : NULL, (void *) database);
}

static VALUE rbpod_database_allocate(VALUE self) {
    Itdb_iTunesDB **database = ALLOC(Itdb_iTunesDB *);

    /* Initialize a new, empty database. */
    *database = itdb_new();

    return rbpod_database_wrap(database, TRUE);
}

void Init_rbpod_database(void) {
    cRbPodDatabase = rb_define_class_under(mRbPod, "Database", rb_cObject);

    rb_define_alloc_func(cRbPodDatabase, rbpod_database_allocate);

    rb_define_method(cRbPodDatabase, "initialize", rbpod_database_initialize, 1);

    rb_define_singleton_method(cRbPodDatabase, "create!", rbpod_database_create, 3);

    rb_define_method(cRbPodDatabase, "mountpoint", rbpod_database_mountpoint_get, 0);
    rb_define_method(cRbPodDatabase, "mountpoint=", rbpod_database_mountpoint_set, 1);

    rb_define_method(cRbPodDatabase, "id", rbpod_database_id_get, 0);
    rb_define_method(cRbPodDatabase, "version", rbpod_database_version_get, 0);
    rb_define_method(cRbPodDatabase, "filename", rbpod_database_filename_get, 0);

    rb_define_method(cRbPodDatabase, "device", rbpod_database_device_get, 0);
    rb_define_method(cRbPodDatabase, "tracks", rbpod_database_tracks_get, 0);
    rb_define_method(cRbPodDatabase, "playlists", rbpod_database_playlists_get, 0);

    rb_define_method(cRbPodDatabase, "synchronized?", rbpod_database_is_synchronized, 0);

    rb_define_method(cRbPodDatabase, "write!", rbpod_database_write, 0);
}

