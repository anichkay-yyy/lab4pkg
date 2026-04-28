#ifndef DB_H
#define DB_H

#include <sqlite3.h>

int db_open(const char *path, sqlite3 **db);
int db_close(sqlite3 *db);
int db_init(sqlite3 *db);
int db_execute(sqlite3 *db, const char *sql);

#endif
