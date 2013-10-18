/* collection.h */

#ifndef RBPOD_COLLECTION_H
#define RBPOD_COLLECTION_H

extern VALUE cRbPodCollection;

void Init_rbpod_collection(void);

inline VALUE rbpod_collection_wrap(GList *list, VALUE type, gboolean freeable);

#endif /* RBPOD_COLLECTION_H */
