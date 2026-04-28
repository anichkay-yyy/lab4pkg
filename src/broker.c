#include "broker.h"
#include <stdio.h>

int broker_insert(sqlite3 *db, const char *surname, const char *address, int birth_year) {
    const char *sql = "INSERT INTO PB_BROKERS (surname, address, birth_year) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, surname, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, address, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, birth_year);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Insert broker failed: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    return (int)sqlite3_last_insert_rowid(db);
}

int broker_update(sqlite3 *db, int id, const char *surname, const char *address, int birth_year) {
    const char *sql = "UPDATE PB_BROKERS SET surname = ?, address = ?, birth_year = ? WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, surname, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, address, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, birth_year);
    sqlite3_bind_int(stmt, 4, id);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int broker_delete(sqlite3 *db, int id) {
    const char *sql = "DELETE FROM PB_BROKERS WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int broker_list(sqlite3 *db) {
    const char *sql = "SELECT id, surname, address, birth_year FROM PB_BROKERS ORDER BY id;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    printf("\n%-5s %-20s %-30s %-10s\n", "ID", "Фамилия", "Адрес", "Год рожд.");
    printf("----------------------------------------------------------------------\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%-5d %-20s %-30s %-10d\n",
               sqlite3_column_int(stmt, 0),
               (const char *)sqlite3_column_text(stmt, 1),
               (const char *)sqlite3_column_text(stmt, 2),
               sqlite3_column_int(stmt, 3));
    }
    sqlite3_finalize(stmt);
    return 0;
}

int broker_get(sqlite3 *db, int id) {
    const char *sql = "SELECT id, surname, address, birth_year FROM PB_BROKERS WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID: %d\nФамилия: %s\nАдрес: %s\nГод рождения: %d\n",
               sqlite3_column_int(stmt, 0),
               (const char *)sqlite3_column_text(stmt, 1),
               (const char *)sqlite3_column_text(stmt, 2),
               sqlite3_column_int(stmt, 3));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    printf("Маклер с ID %d не найден.\n", id);
    return -1;
}

int broker_count(sqlite3 *db) {
    const char *sql = "SELECT COUNT(*) FROM PB_BROKERS;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}
