/* error.h */

#ifndef RBPOD_ERROR_H
#define RBPOD_ERROR_H

RUBY_EXTERN VALUE eRbPodError;

void Init_rbpod_error(void);

inline VALUE rbpod_raise_error(GError *error);

#endif /* RBPOD_ERROR_H */
