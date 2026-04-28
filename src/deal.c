#include "deal.h"
#include <stdio.h>

int deal_insert(sqlite3 *db, const char *deal_date, const char *product_name,
                const char *product_type, int quantity_sold, int broker_id, const char *buyer_firm) {
    const char *sql = "INSERT INTO PB_DEALS (deal_date, product_name, product_type, quantity_sold, broker_id, buyer_firm) "
                      "VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, deal_date, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, product_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, product_type, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, quantity_sold);
    sqlite3_bind_int(stmt, 5, broker_id);
    sqlite3_bind_text(stmt, 6, buyer_firm, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Insert deal failed: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    return (int)sqlite3_last_insert_rowid(db);
}

int deal_update(sqlite3 *db, int id, const char *deal_date, const char *product_name,
                const char *product_type, int quantity_sold, int broker_id, const char *buyer_firm) {
    const char *sql = "UPDATE PB_DEALS SET deal_date = ?, product_name = ?, product_type = ?, "
                      "quantity_sold = ?, broker_id = ?, buyer_firm = ? WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, deal_date, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, product_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, product_type, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, quantity_sold);
    sqlite3_bind_int(stmt, 5, broker_id);
    sqlite3_bind_text(stmt, 6, buyer_firm, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, id);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int deal_delete(sqlite3 *db, int id) {
    const char *sql = "DELETE FROM PB_DEALS WHERE id = ?;";
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

int deal_list(sqlite3 *db, int broker_id) {
    const char *sql;
    sqlite3_stmt *stmt;

    if (broker_id > 0) {
        sql = "SELECT id, deal_date, product_name, product_type, quantity_sold, broker_id, buyer_firm "
              "FROM PB_DEALS WHERE broker_id = ? ORDER BY deal_date;";
    } else {
        sql = "SELECT id, deal_date, product_name, product_type, quantity_sold, broker_id, buyer_firm "
              "FROM PB_DEALS ORDER BY deal_date;";
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    if (broker_id > 0) {
        sqlite3_bind_int(stmt, 1, broker_id);
    }

    printf("\n%-4s %-12s %-18s %-18s %-8s %-10s %-18s\n",
           "ID", "Дата", "Товар", "Вид", "Кол-во", "Маклер ID", "Покупатель");
    printf("--------------------------------------------------------------------------------------------\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%-4d %-12s %-18s %-18s %-8d %-10d %-18s\n",
               sqlite3_column_int(stmt, 0),
               (const char *)sqlite3_column_text(stmt, 1),
               (const char *)sqlite3_column_text(stmt, 2),
               (const char *)sqlite3_column_text(stmt, 3),
               sqlite3_column_int(stmt, 4),
               sqlite3_column_int(stmt, 5),
               (const char *)sqlite3_column_text(stmt, 6));
    }
    sqlite3_finalize(stmt);
    return 0;
}

int deal_count(sqlite3 *db) {
    const char *sql = "SELECT COUNT(*) FROM PB_DEALS;";
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
