/* database.c */

#include "rbpod.h"
#include "device.h"
#include "database.h"
#include "track_collection.h"
#include "playlist_collection.h"

static VALUE rbpod_database_save(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    GError *error = NULL;

    if (itdb_write(database, &error) == FALSE) {
        return rbpod_raise_error(error);
    }

    return self;
}

static VALUE rbpod_database_synchronized_p(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    guint32 nontransferred = itdb_tracks_number_nontransferred(database);
    return BooleanValue(nontransferred == 0);
}

static VALUE rbpod_database_playlists_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return rbpod_playlist_collection_create(self, database->playlists);
}

static VALUE rbpod_database_tracks_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    Itdb_Playlist *playlist = itdb_playlist_mpl(database);
    /* Use the master playlist as the parent for the master track list. */
    VALUE parent = Data_Wrap_Struct(cRbPodPlaylist, NULL, NULL, (void *) playlist);
    return rbpod_track_collection_create(parent, database->tracks);
}

static VALUE rbpod_database_device_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return rbpod_device_create(database->device);
}

static VALUE rbpod_database_filename_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return rb_str_new2(database->filename);
}

static VALUE rbpod_database_version_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return INT2NUM(database->version);
}

static VALUE rbpod_database_id_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return INT2NUM(database->id);
}

static VALUE rbpod_database_mountpoint_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return rb_str_new2(itdb_get_mountpoint(database));
}

static VALUE rbpod_database_mountpoint_set(VALUE self, VALUE path)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    itdb_set_mountpoint(database, StringValueCStr(path));
    return rbpod_database_mountpoint_get(self);
}

static void rbpod_database_deallocate(void *handle)
{
    itdb_free((Itdb_iTunesDB *) handle);
    return;
}

static VALUE rbpod_database_initialize(VALUE self, VALUE mount_point)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    Itdb_iTunesDB *previous = database;
    GError *error = NULL;

    /* Try to parse the database from the given mount point. */
    database = itdb_parse(StringValueCStr(mount_point), &error);

    if (database == NULL) {
        return rbpod_raise_error(error);
    }

    /* Overwrite our old database with the new one. */
    DATA_PTR(self) = database;

    /* Free the old one. */
    itdb_free(previous);

    return self;
}

static VALUE rbpod_database_allocate(VALUE self)
{
    Itdb_iTunesDB *database = itdb_new();
    return Data_Wrap_Struct(cRbPodDatabase, NULL, rbpod_database_deallocate, (void *) database);
}

static VALUE rbpod_database_create(int argc, VALUE *argv, VALUE self)
{
    VALUE mount_point, device_name, model_number, instance;
    gchar *_mount_point, *_device_name, *_model_number;
    GDir *directory = NULL;
    GError *error = NULL;

    if (rb_scan_args(argc, argv, "12", &mount_point, &device_name, &model_number) < 1) {
        rb_raise(eRbPodError, "Invalid arguments.");
        return Qnil;
    }

    /* Reject the mount point immediately if it isn't a string. */
    if (TYPE(mount_point) != T_STRING && RSTRING_LEN(mount_point)) {
        rb_raise(eRbPodError, "Mount point must be a non-empty string.");
        return Qnil;
    }

    /* If we didn't specify a device name, default to 'iPod'. */
    if (NIL_P(device_name) == TRUE) {
        device_name = rb_str_new2("iPod");
    }

    /* If the specified device name isn't a string of at least three characters, bail now. */
    if (TYPE(device_name) != T_STRING || RSTRING_LEN(device_name) < 3) {
        rb_raise(eRbPodError, "Device name should be a string of at least three characters.");
        return Qnil;
    }

    /* If the specified model number is specified but isn't a string, bail now. TODO: Use a regexp, stupid. */
    if (NIL_P(model_number) == FALSE && (TYPE(model_number) != T_STRING || RSTRING_LEN(model_number) < 4)) {
        rb_raise(eRbPodError, "Model number should be a string of at least four characters.");
        return Qnil;
    }

    /* Extract pointers for glib use. */
    _mount_point  = StringValueCStr(mount_point);
    _device_name  = StringValueCStr(device_name);

    /* GPod can function with a NULL model number, however, artwork may not function properly. */
    _model_number = !NIL_P(model_number) ? StringValueCStr(model_number) : NULL;

    /* Check if the mount point is a directory. */
    directory = g_dir_open(_mount_point, 0, &error);

    if (directory == NULL) {
        return rbpod_raise_error(error);
    }

    /* Glib seems to think so... */
    g_dir_close(directory);

    /* Initialize the iPod at this mount point, with this device name and model number. */
    if (itdb_init_ipod(_mount_point, _model_number, _device_name, &error) == FALSE) {
        return rbpod_raise_error(error);
    }

    instance = rbpod_database_allocate(cRbPodDatabase);

    /* Return an instance of this class using the newly created database. */
    return rbpod_database_initialize(instance, mount_point);
}

void Init_rbpod_database(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    cRbPodDatabase = rb_define_class_under(mRbPod, "Database", rb_cObject);

    rb_define_alloc_func(cRbPodDatabase, rbpod_database_allocate);

    rb_define_method(cRbPodDatabase, "initialize", rbpod_database_initialize, 1);

    rb_define_singleton_method(cRbPodDatabase, "create!", rbpod_database_create, -1);

    rb_define_method(cRbPodDatabase, "mountpoint", rbpod_database_mountpoint_get, 0);
    rb_define_method(cRbPodDatabase, "mountpoint=", rbpod_database_mountpoint_set, 1);

    rb_define_method(cRbPodDatabase, "id", rbpod_database_id_get, 0);
    rb_define_method(cRbPodDatabase, "version", rbpod_database_version_get, 0);
    rb_define_method(cRbPodDatabase, "filename", rbpod_database_filename_get, 0);

    rb_define_method(cRbPodDatabase, "device", rbpod_database_device_get, 0);
    rb_define_method(cRbPodDatabase, "tracks", rbpod_database_tracks_get, 0);
    rb_define_method(cRbPodDatabase, "playlists", rbpod_database_playlists_get, 0);

    rb_define_method(cRbPodDatabase, "synchronized?", rbpod_database_synchronized_p, 0);

    rb_define_method(cRbPodDatabase, "save!", rbpod_database_save, 0);
}

