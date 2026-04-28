#ifndef BROKER_H
#define BROKER_H

#include <sqlite3.h>

int broker_insert(sqlite3 *db, const char *surname, const char *address, int birth_year);
int broker_update(sqlite3 *db, int id, const char *surname, const char *address, int birth_year);
int broker_delete(sqlite3 *db, int id);
int broker_list(sqlite3 *db);
int broker_get(sqlite3 *db, int id);
int broker_count(sqlite3 *db);

#endif
