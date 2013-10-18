/* device.h */

#ifndef RBPOD_DEVICE_H
#define RBPOD_DEVICE_H

extern VALUE cRbPodDevice;

void Init_rbpod_device(void);

inline VALUE rbpod_device_wrap(Itdb_Device *device, gboolean freeable);

#endif /* RBPOD_DEVICE_H */
