/* device.c */

#include "rbpod.h"

/*
 * call-seq:
 *     supports_chapter_images?() -> Boolean
 *
 * Returns true of false if the device supports chapter images.
 */
static VALUE rbpod_device_chapter_image_support_p(VALUE self)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    return BooleanValue(itdb_device_supports_chapter_image(device));
}

/*
 * call-seq:
 *     supports_podcasts?() -> Boolean
 *
 * Returns true of false if the device supports podcasts.
 */
static VALUE rbpod_device_podcast_support_p(VALUE self)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    return BooleanValue(itdb_device_supports_podcast(device));
}

/*
 * call-seq:
 *     supports_artwork?() -> Boolean
 *
 * Returns true of false if the device supports artwork.
 */
static VALUE rbpod_device_artwork_support_p(VALUE self)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    return BooleanValue(itdb_device_supports_artwork(device));
}

/*
 * call-seq:
 *     supports_videos?() -> Boolean
 *
 * Returns true of false if the device supports videos.
 */
static VALUE rbpod_device_video_support_p(VALUE self)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    return BooleanValue(itdb_device_supports_video(device));
}

/*
 * call-seq:
 *     supports_photos?() -> Boolean
 *
 * Returns true of false if the device supports photos.
 */
static VALUE rbpod_device_photo_support_p(VALUE self)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    return BooleanValue(itdb_device_supports_photo(device));
}

/*
 * call-seq:
 *     model_name() -> String
 *
 * Returns the model name of the device.
 */
static VALUE rbpod_device_model_name_get(VALUE self)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    const Itdb_IpodInfo *info = itdb_device_get_ipod_info(device);
    return rb_str_new2(itdb_info_get_ipod_model_name_string(info->ipod_model));
}

/*
 * call-seq:
 *     model_number() -> String
 *
 * Returns the model number of the device.
 */
static VALUE rbpod_device_model_number_get(VALUE self)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    const Itdb_IpodInfo *info = itdb_device_get_ipod_info(device);
    return rb_str_new2(info->model_number);
}

/*
 * call-seq:
 *     generation() -> String
 *
 * Returns the generation of the device.
 */
static VALUE rbpod_device_generation_get(VALUE self)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    const Itdb_IpodInfo *info = itdb_device_get_ipod_info(device);
    return rb_str_new2(itdb_info_get_ipod_generation_string(info->ipod_generation));
}

/*
 * call-seq:
 *     capacity() -> Float
 *
 * Returns the capacity of the device in gigabytes (GB).
 */
static VALUE rbpod_device_capacity_get(VALUE self)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    const Itdb_IpodInfo *info = itdb_device_get_ipod_info(device);
    return DBL2NUM(info->capacity);
}

/*
 * call-seq:
 *     uuid() -> String
 *
 * Returns the UUID of the device.
 */
static VALUE rbpod_device_uuid_get(VALUE self)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    const gchar *uuid = itdb_device_get_uuid(device);
    return (uuid == NULL) ? Qnil : rb_str_new2(uuid);
}

/*
 * call-seq:
 *     [](key) -> String
 *
 * Returns the SysInfo entry for the given key, or nil.
 */
static VALUE rbpod_device_sysinfo_get(VALUE self, VALUE key)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    gchar *value = NULL;
    VALUE result;

    value = itdb_device_get_sysinfo(device, StringValueCStr(key));
    result = (value == NULL) ? Qnil : rb_str_new2(value);

    g_free(value);
    return result;

}

/*
 * call-seq:
 *     []=(key, value) -> nil
 *
 * Sets the given SysInfo entry to a value and returns nil.
 */
static VALUE rbpod_device_sysinfo_set(VALUE self, VALUE key, VALUE value)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    gchar *_value, *_key;

    _key   = StringValueCStr(key);
    _value = !NIL_P(value) ? StringValueCStr(value) : NULL;

    itdb_device_set_sysinfo(device, _key, _value);

    return Qnil;
}

/*
 * call-seq:
 *     save!() -> nil
 *
 * Writes the SysInfo data back to the device. Call this if you change any entries.
 */
static VALUE rbpod_device_sysinfo_save(VALUE self)
{
    Itdb_Device *device = TYPED_DATA_PTR(self, Itdb_Device);
    GError *error = NULL;

    if (itdb_device_write_sysinfo(device, &error) == FALSE) {
        return rbpod_raise_error(error);
    }

    return Qnil;
}

/*
 * call-seq:
 *     initialize(database) -> RbPod::Device
 *
 * Creates an RbPod::Device for a given RbPod::Database. You shouldn't have to call this.
 */
static VALUE rbpod_device_initialize(VALUE self, VALUE database)
{
    Itdb_iTunesDB *_database = TYPED_DATA_PTR(database, Itdb_iTunesDB);

    /* Make sure we were given an instance of a RbPod::Database. */
    if (rb_obj_is_instance_of(database, cRbPodDatabase) == FALSE) {
        rb_raise(eRbPodError, "Given database must be an instance of RbPod::Database: %s", StringValueCStr(database));
        return Qnil;
    }

    /* The database pointer and it's device should both be non-NULL. */
    if (_database == NULL || _database->device == NULL) {
        rb_raise(eRbPodError, "Given database and/or device was NULL.");
        return Qnil;
    }

    /* Attach our data pointer to this database's backing device. */
    DATA_PTR(self) = _database->device;

    return self;
}

void Init_rbpod_device(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    cRbPodDevice = rb_define_class_under(mRbPod, "Device", rb_cObject);

    rb_define_method(cRbPodDevice, "initialize", rbpod_device_initialize, 1);

    rb_define_method(cRbPodDevice, "[]", rbpod_device_sysinfo_get, 1);
    rb_define_method(cRbPodDevice, "[]=", rbpod_device_sysinfo_set, 2);
    rb_define_method(cRbPodDevice, "save!", rbpod_device_sysinfo_save, 0);

    rb_define_method(cRbPodDevice, "uuid", rbpod_device_uuid_get, 0);
    rb_define_method(cRbPodDevice, "capacity", rbpod_device_capacity_get, 0);
    rb_define_method(cRbPodDevice, "generation", rbpod_device_generation_get, 0);
    rb_define_method(cRbPodDevice, "model_name", rbpod_device_model_name_get, 0);
    rb_define_method(cRbPodDevice, "model_number", rbpod_device_model_number_get, 0);

    rb_define_method(cRbPodDevice, "supports_photos?", rbpod_device_photo_support_p, 0);
    rb_define_method(cRbPodDevice, "supports_videos?", rbpod_device_video_support_p, 0);
    rb_define_method(cRbPodDevice, "supports_artwork?", rbpod_device_artwork_support_p, 0);
    rb_define_method(cRbPodDevice, "supports_podcasts?", rbpod_device_podcast_support_p, 0);
    rb_define_method(cRbPodDevice, "supports_chapter_images?", rbpod_device_chapter_image_support_p, 0);

    return;
}

