#include "auth.h"
#include <stdio.h>
#include <string.h>

unsigned long hash_password(const char *password) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*password++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

int auth_login(sqlite3 *db, const char *username, const char *password, User *user) {
    const char *sql = "SELECT id, username, password_hash, role, broker_id FROM PB_USERS WHERE username = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare login query: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *stored_hash = (const char *)sqlite3_column_text(stmt, 2);
        unsigned long computed_hash = hash_password(password);
        char hash_str[32];
        snprintf(hash_str, sizeof(hash_str), "%lu", computed_hash);

        if (strcmp(hash_str, stored_hash) == 0) {
            user->id = sqlite3_column_int(stmt, 0);
            strncpy(user->username, (const char *)sqlite3_column_text(stmt, 1), sizeof(user->username) - 1);
            user->username[sizeof(user->username) - 1] = '\0';
            strncpy(user->role, (const char *)sqlite3_column_text(stmt, 3), sizeof(user->role) - 1);
            user->role[sizeof(user->role) - 1] = '\0';
            user->broker_id = sqlite3_column_type(stmt, 4) == SQLITE_NULL ? 0 : sqlite3_column_int(stmt, 4);
            sqlite3_finalize(stmt);
            return 0;
        }
    }

    sqlite3_finalize(stmt);
    return -1;
}

int auth_register(sqlite3 *db, const char *username, const char *password, const char *role, int broker_id) {
    const char *sql = "INSERT INTO PB_USERS (username, password_hash, role, broker_id) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare register query: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    unsigned long hash = hash_password(password);
    char hash_str[32];
    snprintf(hash_str, sizeof(hash_str), "%lu", hash);

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hash_str, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, role, -1, SQLITE_STATIC);
    if (broker_id > 0) {
        sqlite3_bind_int(stmt, 4, broker_id);
    } else {
        sqlite3_bind_null(stmt, 4);
    }

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}
