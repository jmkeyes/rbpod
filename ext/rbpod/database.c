/* database.c */

#include "rbpod.h"

/*
 * call-seq:
 *     save!() -> nil
 *
 * Saves any changes made to the database.
 */
static VALUE rbpod_database_save(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    GError *error = NULL;

    if (itdb_write(database, &error) == FALSE) {
        return rbpod_raise_error(error);
    }

    /* TODO: Iterate through all tracks, adding/removing from master playlist and filesystem. */

    /* TODO: Iterate through playlists, save them if they've been modified. */

    /* TODO: Save any modifications to the backing device. */

    return Qnil;
}

/*
 * call-seq:
 *     synchronized?() -> Boolean
 *
 * Returns true or false depending if any changes have been made, but not saved.
 */
static VALUE rbpod_database_synchronized_p(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    guint32 nontransferred = itdb_tracks_number_nontransferred(database);
    return BooleanValue(nontransferred == 0);
}

/*
 * call-seq:
 *     playlists() -> Enumerator
 *
 * Returns an enumerator of all playlists added to this database.
 */
static VALUE rbpod_database_playlists_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    GList *current = NULL;

    RETURN_CUSTOMIZED_ENUMERATOR(self, 0, 0, mRbPodPlaylistCollection);

    for (current = database->playlists; current != NULL; current = g_list_next(current)) {
        VALUE playlist = rb_class_new_instance_with_data(0, NULL, cRbPodPlaylist, current->data);
        rb_yield(playlist);
    }

    return Qnil;
}

/*
 * call-seq:
 *     tracks() -> Enumerator
 *
 * Returns an enumerator of all tracks added to this database.
 */
static VALUE rbpod_database_tracks_get(VALUE self)
{
    VALUE known_playlists = rbpod_database_playlists_get(self);
    VALUE master_playlist = rb_funcall(known_playlists, rb_intern("master"), 0);
    return rb_funcall(master_playlist, rb_intern("tracks"), 0);
}

/*
 * call-seq:
 *     device() -> RbPod::Device
 *
 * Returns the device backing this database.
 */
static VALUE rbpod_database_device_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    VALUE mount_point = rb_str_new2(itdb_get_mountpoint(database));
    return rb_class_new_instance(1, &mount_point, cRbPodDevice);
}

/*
 * call-seq:
 *     filename() -> String
 *
 * Returns the path on the file system to the database.
 */
static VALUE rbpod_database_filename_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    VALUE file_path = rb_str_new2(database->filename);
    return rb_class_new_instance(1, &file_path, rb_cPathname);
}

/*
 * call-seq:
 *     version() -> Fixnum
 *
 * Returns the version number of the database.
 */
static VALUE rbpod_database_version_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return INT2NUM(database->version);
}

/*
 * :nodoc:
 */
static VALUE rbpod_database_id_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    return INT2NUM(database->id);
}

/*
 * call-seq:
 *     mountpoint() -> String
 *
 * Returns the location of the mount point this database was parsed from.
 */
static VALUE rbpod_database_mountpoint_get(VALUE self)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    VALUE mount_point = rb_str_new2(itdb_get_mountpoint(database));
    return rb_class_new_instance(1, &mount_point, rb_cPathname);
}

/*
 * :nodoc:
 */
static VALUE rbpod_database_mountpoint_set(VALUE self, VALUE path)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    itdb_set_mountpoint(database, StringValueCStr(path));
    return Qnil;
}

static void rbpod_database_deallocate(void *handle)
{
    itdb_free((Itdb_iTunesDB *) handle);
    return;
}

/*
 * call-seq:
 *     initialize(mount_point) -> RbPod::Database
 *
 * Loads a new database parsed from the given mount point.
 */
static VALUE rbpod_database_initialize(VALUE self, VALUE mount_point)
{
    Itdb_iTunesDB *database = TYPED_DATA_PTR(self, Itdb_iTunesDB);
    Itdb_iTunesDB *previous = database;
    GError *error = NULL;

    /* Check if the mount point is a directory. */
    if (rb_file_directory_p(rb_cFile, mount_point) != Qtrue) {
        rb_raise(eRbPodError, "The mount point must be a directory!");
        return Qnil;
    }

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

/*
 * call-seq:
 *     create!(mount_point) -> RbPod::Database
 *     create!(mount_point, device_name) -> RbPod::Database
 *     create!(mount_point, device_name, model_number) -> RbPod::Database
 *
 * Creates a new database on the file system and loads it.
 */
static VALUE rbpod_database_create(int argc, VALUE *argv, VALUE self)
{
    VALUE mount_point, device_name, model_number, regexp;
    gchar *_mount_point, *_device_name, *_model_number;
    const char *mregex = "(?i)^[xM]?[A-Z][0-9]{3}$";
    GError *error = NULL;

    if (rb_scan_args(argc, argv, "12", &mount_point, &device_name, &model_number) < 1) {
        rb_raise(eRbPodError, "Invalid arguments.");
        return Qnil;
    }

    /* Check if the mount point is a directory. */
    if (rb_file_directory_p(rb_cFile, mount_point) != Qtrue) {
        rb_raise(eRbPodError, "The mount point must be a directory!");
        return Qnil;
    }

    /* If a device name was given, ensure it's a string. */
    if (RTEST(device_name)) {
        Check_Type(device_name, T_STRING);

        if (RSTRING_LEN(device_name) < 4) {
            rb_raise(eRbPodError, "Device name must be a string of at least four characters.");
            return Qnil;
        }
    }

    /* If a model number was given, ensure it's a string. */
    if (RTEST(model_number)) {
        Check_Type(model_number, T_STRING);

        /* TODO: Precompile regex at initialization. */
        regexp  = rb_reg_new(mregex, strlen(mregex), 0);

        if (RTEST(rb_reg_match(regexp, model_number)) == FALSE) {
            rb_raise(eRbPodError, "Model number must be a string matching: %s", mregex);
            return Qnil;
        }
    }

    /* Extract pointers for glib use. */
    _mount_point  = StringValueCStr(mount_point);

    /* GPod will use 'iPod' as the device name if it wasn't specified. */
    _device_name  = !NIL_P(device_name) ? StringValueCStr(device_name) : NULL;

    /* GPod can function with a NULL model number, however, artwork may not function properly. */
    _model_number = !NIL_P(model_number) ? StringValueCStr(model_number) : NULL;

    /* Initialize the iPod at this mount point, with this device name and model number. */
    if (itdb_init_ipod(_mount_point, _model_number, _device_name, &error) == FALSE) {
        return rbpod_raise_error(error);
    }

    /* Return an instance of this class using the newly created database. */
    return rb_class_new_instance(1, &mount_point, cRbPodDatabase);
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

    rb_define_private_method(cRbPodDatabase, "id", rbpod_database_id_get, 0);
    rb_define_private_method(cRbPodDatabase, "mountpoint=", rbpod_database_mountpoint_set, 1);

    rb_define_method(cRbPodDatabase, "mountpoint", rbpod_database_mountpoint_get, 0);

    rb_define_method(cRbPodDatabase, "version", rbpod_database_version_get, 0);
    rb_define_method(cRbPodDatabase, "filename", rbpod_database_filename_get, 0);

    rb_define_method(cRbPodDatabase, "device", rbpod_database_device_get, 0);

    rb_define_method(cRbPodDatabase, "tracks", rbpod_database_tracks_get, 0);
    rb_define_method(cRbPodDatabase, "playlists", rbpod_database_playlists_get, 0);

    rb_define_method(cRbPodDatabase, "synchronized?", rbpod_database_synchronized_p, 0);

    rb_define_method(cRbPodDatabase, "save!", rbpod_database_save, 0);
}

