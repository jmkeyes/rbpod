/* database.c */

#include "rbpod.h"
#include "track.h"
#include "device.h"
#include "playlist.h"
#include "database.h"
#include "collection.h"

VALUE cRbPodDatabase;

static VALUE rbpod_database_write(VALUE self) {
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    gboolean success = itdb_write(database, NULL); /* TODO: Improve error handling. */
    return BooleanValue(success);
}

static VALUE rbpod_database_is_synchronized(VALUE self) {
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    guint32 nontransferred = itdb_tracks_number_nontransferred(database);
    return BooleanValue(nontransferred == 0);
}

static VALUE rbpod_database_playlists_get(VALUE self) {
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    VALUE collection = rbpod_collection_create(database->playlists, cRbPodPlaylist);
    rb_extend_object(collection, mRbPodPlaylistCollection);
    return collection;
}

static VALUE rbpod_database_tracks_get(VALUE self) {
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    VALUE collection = rbpod_collection_create(database->tracks, cRbPodTrack);
    rb_extend_object(collection, mRbPodTrackCollection);
    return collection;
}

static VALUE rbpod_database_device_get(VALUE self) {
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return rbpod_device_create(database->device);
}

static VALUE rbpod_database_filename_get(VALUE self) {
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return rb_str_new2(database->filename);
}

static VALUE rbpod_database_version_get(VALUE self) {
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return INT2NUM(database->version);
}

static VALUE rbpod_database_id_get(VALUE self) {
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return INT2NUM(database->id);
}

static VALUE rbpod_database_mountpoint_get(VALUE self) {
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return rb_str_new2(itdb_get_mountpoint(database));
}

static VALUE rbpod_database_mountpoint_set(VALUE self, VALUE path) {
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    itdb_set_mountpoint(database, StringValueCStr(path));
    return rbpod_database_mountpoint_get(self);
}

static void rbpod_database_deallocate(void *handle) {
    itdb_free((Itdb_iTunesDB *) handle);
    return;
}

static VALUE rbpod_database_create(VALUE self, VALUE device_name, VALUE mount_point, VALUE model_number) {
    gchar *_mount_point, *_model_number, *_device_name;
    Itdb_iTunesDB *database = NULL;

    _device_name  = StringValueCStr(device_name);
    _mount_point  = StringValueCStr(mount_point);

    /* GPod can function with a NULL model number, however, artwork will not function. */
    _model_number = !NIL_P(model_number) ? StringValueCStr(model_number) : NULL;

    /* Initialize the iPod at this mount point, with this device name and model number. */
    /* TODO: Improve error handling. */
    if (itdb_init_ipod(_mount_point, _model_number, _device_name, NULL) == FALSE) {
        rb_raise(eRbPodError, "Unable to format this iPod.");
        return Qfalse;
    }

    /* Parse the newly created database. */
    /* TODO: Improve error handling. */
    database = itdb_parse(_mount_point, NULL);

    if (database == NULL) {
        rb_raise(eRbPodError, "Not an iPod mount point.");
        return Qnil;
    }

    /* Return an instance of this class using the newly created database. */
    return Data_Wrap_Struct(cRbPodDatabase, NULL, rbpod_database_deallocate, (void *) database);
}

static VALUE rbpod_database_initialize(VALUE self, VALUE mount_point) {
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);

    /* Try to parse the database from the given mount point. */
    /* TODO: Improve error handling. */
    database = itdb_parse(StringValueCStr(mount_point), NULL);

    /* The given mount point was not an iPod mount point. */
    if (database == NULL) {
        rb_raise(eRbPodError, "This is not an iPod mount point.");
        return Qnil;
    }

    DATA_PTR(self) = database;

    return self;
}

static VALUE rbpod_database_allocate(VALUE self) {
    Itdb_iTunesDB *database = itdb_new();
    return Data_Wrap_Struct(cRbPodDatabase, NULL, rbpod_database_deallocate, (void *) database);
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

