#include "functions.h"
#include <stdio.h>

int fn_update_broker_stats(sqlite3 *db, int deal_id) {
    const char *sql =
        "SELECT d.broker_id, d.quantity_sold, d.quantity_sold * g.unit_price "
        "FROM PB_DEALS d "
        "JOIN PB_GOODS g ON d.product_name = g.name AND d.product_type = g.type "
        "WHERE d.id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, deal_id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        fprintf(stderr, "Сделка с ID %d не найдена.\n", deal_id);
        sqlite3_finalize(stmt);
        return -1;
    }

    int broker_id = sqlite3_column_int(stmt, 0);
    int qty = sqlite3_column_int(stmt, 1);
    double sum = sqlite3_column_double(stmt, 2);
    sqlite3_finalize(stmt);

    const char *upsert =
        "INSERT INTO PB_BROKER_STATS (broker_id, total_quantity_sold, total_deal_sum) "
        "VALUES (?, ?, ?) "
        "ON CONFLICT(broker_id) DO UPDATE SET "
        "total_quantity_sold = total_quantity_sold + excluded.total_quantity_sold, "
        "total_deal_sum = total_deal_sum + excluded.total_deal_sum;";

    if (sqlite3_prepare_v2(db, upsert, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, broker_id);
    sqlite3_bind_int(stmt, 2, qty);
    sqlite3_bind_double(stmt, 3, sum);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        printf("Статистика маклера (ID %d) обновлена: +%d ед., +%.2f руб.\n", broker_id, qty, sum);
        return 0;
    }
    return -1;
}

int fn_consolidate_goods(sqlite3 *db, const char *period_date) {
    int rc;

    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return -1;

    const char *update_sql =
        "UPDATE PB_GOODS SET quantity_supplied = quantity_supplied - COALESCE("
        "    (SELECT SUM(d.quantity_sold) FROM PB_DEALS d "
        "     WHERE d.product_name = PB_GOODS.name AND d.product_type = PB_GOODS.type "
        "     AND d.deal_date <= ?), 0"
        ") WHERE EXISTS ("
        "    SELECT 1 FROM PB_DEALS d "
        "    WHERE d.product_name = PB_GOODS.name AND d.product_type = PB_GOODS.type "
        "    AND d.deal_date <= ?"
        ");";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, update_sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, period_date, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, period_date, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Ошибка обновления товаров: %s\n", sqlite3_errmsg(db));
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return -1;
    }

    const char *delete_sql = "DELETE FROM PB_DEALS WHERE deal_date <= ?;";
    if (sqlite3_prepare_v2(db, delete_sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, period_date, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    int deleted = sqlite3_changes(db);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Ошибка удаления сделок: %s\n", sqlite3_errmsg(db));
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return -1;
    }

    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    printf("Консолидация выполнена на дату %s. Удалено сделок: %d\n", period_date, deleted);
    return 0;
}

int fn_broker_deals_on_date(sqlite3 *db, const char *date, int broker_id) {
    const char *sql =
        "SELECT d.id, d.deal_date, d.product_name, d.product_type, "
        "d.quantity_sold, d.buyer_firm, d.quantity_sold * g.unit_price AS deal_sum "
        "FROM PB_DEALS d "
        "JOIN PB_GOODS g ON d.product_name = g.name AND d.product_type = g.type "
        "WHERE d.deal_date = ? AND d.broker_id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, date, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, broker_id);

    printf("\n--- Сделки маклера (ID %d) на дату %s ---\n", broker_id, date);
    printf("%-4s %-12s %-18s %-18s %-8s %-18s %-12s\n",
           "ID", "Дата", "Товар", "Вид", "Кол-во", "Покупатель", "Сумма");
    printf("--------------------------------------------------------------------------------------------\n");

    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%-4d %-12s %-18s %-18s %-8d %-18s %-12.2f\n",
               sqlite3_column_int(stmt, 0),
               (const char *)sqlite3_column_text(stmt, 1),
               (const char *)sqlite3_column_text(stmt, 2),
               (const char *)sqlite3_column_text(stmt, 3),
               sqlite3_column_int(stmt, 4),
               (const char *)sqlite3_column_text(stmt, 5),
               sqlite3_column_double(stmt, 6));
        found = 1;
    }
    if (!found) printf("Нет сделок.\n");

    sqlite3_finalize(stmt);
    return 0;
}
