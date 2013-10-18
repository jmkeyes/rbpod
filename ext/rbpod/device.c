/* device.c */

#include "rbpod.h"
#include "device.h"

VALUE cRbPodDevice;

static inline Itdb_Device *rbpod_device_unwrap(VALUE self) {
    Itdb_Device *device = NULL;
    Data_Get_Struct(self, Itdb_Device, device);
    return device;
}

static VALUE rbpod_device_has_chapter_image_support(VALUE self) {
    Itdb_Device *device = rbpod_device_unwrap(self);
    return BooleanValue(itdb_device_supports_chapter_image(device));
}

static VALUE rbpod_device_has_podcast_support(VALUE self) {
    Itdb_Device *device = rbpod_device_unwrap(self);
    return BooleanValue(itdb_device_supports_podcast(device));
}

static VALUE rbpod_device_has_artwork_support(VALUE self) {
    Itdb_Device *device = rbpod_device_unwrap(self);
    return BooleanValue(itdb_device_supports_artwork(device));
}

static VALUE rbpod_device_has_video_support(VALUE self) {
    Itdb_Device *device = rbpod_device_unwrap(self);
    return BooleanValue(itdb_device_supports_video(device));
}

static VALUE rbpod_device_has_photo_support(VALUE self) {
    Itdb_Device *device = rbpod_device_unwrap(self);
    return BooleanValue(itdb_device_supports_photo(device));
}

static VALUE rbpod_device_model_name_get(VALUE self) {
    Itdb_Device *device = rbpod_device_unwrap(self);
    Itdb_IpodInfo *info = itdb_device_get_ipod_info(device);
    return rb_str_new2(itdb_info_get_ipod_model_name_string(info->ipod_model));
}

static VALUE rbpod_device_model_number_get(VALUE self) {
    Itdb_Device *device = rbpod_device_unwrap(self);
    Itdb_IpodInfo *info = itdb_device_get_ipod_info(device);
    return rb_str_new2(info->model_number);
}

static VALUE rbpod_device_generation_get(VALUE self) {
    Itdb_Device *device = rbpod_device_unwrap(self);
    const Itdb_IpodInfo *info = itdb_device_get_ipod_info(device);
    return rb_str_new2(itdb_info_get_ipod_generation_string(info->ipod_generation));
}

static VALUE rbpod_device_capacity_get(VALUE self) {
    Itdb_Device *device = rbpod_device_unwrap(self);
    const Itdb_IpodInfo *info = itdb_device_get_ipod_info(device);
    return rb_float_new(info->capacity);
}

static VALUE rbpod_device_sysinfo_get(VALUE self, VALUE key) {
    Itdb_Device *device = rbpod_device_unwrap(self);
    gchar *value = NULL;
    VALUE result;

    value = itdb_device_get_sysinfo(device, StringValueCStr(key));
    result = (value == NULL) ? Qnil : rb_str_new2(value);

    g_free(value);
    return result;

}

static VALUE rbpod_device_sysinfo_set(VALUE self, VALUE key, VALUE value) {
    Itdb_Device *device = rbpod_device_unwrap(self);
    gchar *_value, *_key;

    _key   = StringValueCStr(key);
    _value = !NIL_P(value) ? StringValueCStr(value) : NULL;

    itdb_device_set_sysinfo(device, _key, _value);

    return rbpod_device_sysinfo_get(self, _key);
}

static VALUE rbpod_device_sysinfo_write(VALUE self) {
    Itdb_Device *device = rbpod_device_unwrap(self);
    /* TODO: Improve error handling. */
    return BooleanValue(itdb_device_write_sysinfo(device, NULL));
}

static VALUE rbpod_device_initialize(VALUE self) {
    /* Private. */
    return self;
}

static void rbpod_device_deallocate(void *handle) {
    itdb_device_free((Itdb_Device *) handle);
}

inline VALUE rbpod_device_wrap(Itdb_Device *device, gboolean freeable) {
    return Data_Wrap_Struct(cRbPodDevice, NULL, freeable ? rbpod_device_deallocate : NULL, (void *) device);
}

static VALUE rbpod_device_allocate(VALUE self) {
    return rbpod_device_wrap(itdb_device_new(), TRUE);
}

void Init_rbpod_device(void) {
    cRbPodDevice = rb_define_class_under(mRbPod, "Device", rb_cObject);

    rb_define_alloc_func(cRbPodDevice, rbpod_device_allocate);

    /* We don't want to explicitly create these, so mark the constructor as private. */
    rb_define_private_method(cRbPodDevice, "initialize", rbpod_device_initialize, 0);

    rb_define_method(cRbPodDevice, "[]", rbpod_device_sysinfo_get, 1);
    rb_define_method(cRbPodDevice, "[]=", rbpod_device_sysinfo_set, 2);
    rb_define_method(cRbPodDevice, "save!", rbpod_device_sysinfo_write, 0);

    rb_define_method(cRbPodDevice, "capacity", rbpod_device_capacity_get, 0);
    rb_define_method(cRbPodDevice, "generation", rbpod_device_generation_get, 0);
    rb_define_method(cRbPodDevice, "model_name", rbpod_device_model_name_get, 0);
    rb_define_method(cRbPodDevice, "model_number", rbpod_device_model_number_get, 0);

    rb_define_method(cRbPodDevice, "supports_photos?", rbpod_device_has_photo_support, 0);
    rb_define_method(cRbPodDevice, "supports_videos?", rbpod_device_has_video_support, 0);
    rb_define_method(cRbPodDevice, "supports_artwork?", rbpod_device_has_artwork_support, 0);
    rb_define_method(cRbPodDevice, "supports_podcasts?", rbpod_device_has_podcast_support, 0);
    rb_define_method(cRbPodDevice, "supports_chapter_images?", rbpod_device_has_chapter_image_support, 0);

    return;
}

