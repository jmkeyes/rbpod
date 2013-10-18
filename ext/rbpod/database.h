/* database.h */

#ifndef RBPOD_DATABASE_H
#define RBPOD_DATABASE_H

extern VALUE cRbPodDatabase;

void Init_rbpod_database(void);

inline VALUE rbpod_database_wrap(Itdb_iTunesDB **database, gboolean freeable);

#endif /* RBPOD_DATABASE_H */
