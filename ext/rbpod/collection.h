/* collection.h */

#ifndef RBPOD_COLLECTION_H
#define RBPOD_COLLECTION_H

RUBY_EXTERN VALUE cRbPodCollection;

void Init_rbpod_collection(void);

inline VALUE rbpod_collection_create(GList *list, VALUE type);

#endif /* RBPOD_COLLECTION_H */
