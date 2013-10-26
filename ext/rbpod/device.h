/* device.h */

#ifndef RBPOD_DEVICE_H
#define RBPOD_DEVICE_H

extern VALUE cRbPodDevice;

void Init_rbpod_device(void);

inline VALUE rbpod_device_create(Itdb_Device *device);

#endif /* RBPOD_DEVICE_H */
