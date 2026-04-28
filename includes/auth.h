#ifndef AUTH_H
#define AUTH_H

#include <sqlite3.h>

typedef struct {
    int id;
    char username[64];
    char role[16];
    int broker_id;
} User;

unsigned long hash_password(const char *password);
int auth_login(sqlite3 *db, const char *username, const char *password, User *user);
int auth_register(sqlite3 *db, const char *username, const char *password, const char *role, int broker_id);

#endif
