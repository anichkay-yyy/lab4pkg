#include "goods.h"
#include <stdio.h>

int goods_insert(sqlite3 *db, const char *name, const char *type, double unit_price,
                 const char *supplier_firm, const char *expiration_date, int quantity_supplied) {
    const char *sql = "INSERT INTO PB_GOODS (name, type, unit_price, supplier_firm, expiration_date, quantity_supplied) "
                      "VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, type, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, unit_price);
    sqlite3_bind_text(stmt, 4, supplier_firm, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, expiration_date, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, quantity_supplied);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Insert goods failed: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    return (int)sqlite3_last_insert_rowid(db);
}

int goods_update(sqlite3 *db, int id, const char *name, const char *type, double unit_price,
                 const char *supplier_firm, const char *expiration_date, int quantity_supplied) {
    const char *sql = "UPDATE PB_GOODS SET name = ?, type = ?, unit_price = ?, supplier_firm = ?, "
                      "expiration_date = ?, quantity_supplied = ? WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, type, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, unit_price);
    sqlite3_bind_text(stmt, 4, supplier_firm, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, expiration_date, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, quantity_supplied);
    sqlite3_bind_int(stmt, 7, id);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int goods_delete(sqlite3 *db, int id) {
    const char *sql = "DELETE FROM PB_GOODS WHERE id = ?;";
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

int goods_list(sqlite3 *db) {
    const char *sql = "SELECT id, name, type, unit_price, supplier_firm, expiration_date, quantity_supplied "
                      "FROM PB_GOODS ORDER BY id;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    printf("\n%-4s %-18s %-18s %-10s %-20s %-12s %-8s\n",
           "ID", "Название", "Вид", "Цена", "Поставщик", "Срок годн.", "Кол-во");
    printf("--------------------------------------------------------------------------------------------\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%-4d %-18s %-18s %-10.2f %-20s %-12s %-8d\n",
               sqlite3_column_int(stmt, 0),
               (const char *)sqlite3_column_text(stmt, 1),
               (const char *)sqlite3_column_text(stmt, 2),
               sqlite3_column_double(stmt, 3),
               (const char *)sqlite3_column_text(stmt, 4),
               (const char *)sqlite3_column_text(stmt, 5),
               sqlite3_column_int(stmt, 6));
    }
    sqlite3_finalize(stmt);
    return 0;
}

int goods_count(sqlite3 *db) {
    const char *sql = "SELECT COUNT(*) FROM PB_GOODS;";
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
