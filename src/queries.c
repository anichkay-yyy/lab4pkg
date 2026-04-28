#include "queries.h"
#include <stdio.h>

int query_product_sales_by_period(sqlite3 *db, const char *product_name,
                                  const char *date_from, const char *date_to) {
    const char *sql =
        "SELECT d.product_name, SUM(d.quantity_sold), SUM(d.quantity_sold * g.unit_price) "
        "FROM PB_DEALS d "
        "JOIN PB_GOODS g ON d.product_name = g.name AND d.product_type = g.type "
        "WHERE d.product_name = ? AND d.deal_date BETWEEN ? AND ? "
        "GROUP BY d.product_name;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, product_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, date_from, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, date_to, -1, SQLITE_STATIC);

    printf("\n--- Продажи товара '%s' за период %s - %s ---\n", product_name, date_from, date_to);
    printf("%-20s %-15s %-15s\n", "Товар", "Кол-во", "Сумма");
    printf("--------------------------------------------------\n");

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%-20s %-15d %-15.2f\n",
               (const char *)sqlite3_column_text(stmt, 0),
               sqlite3_column_int(stmt, 1),
               sqlite3_column_double(stmt, 2));
    } else {
        printf("Нет данных.\n");
    }

    sqlite3_finalize(stmt);
    return 0;
}

int query_product_buyers(sqlite3 *db, const char *product_name) {
    const char *sql =
        "SELECT d.buyer_firm, SUM(d.quantity_sold), SUM(d.quantity_sold * g.unit_price) "
        "FROM PB_DEALS d "
        "JOIN PB_GOODS g ON d.product_name = g.name AND d.product_type = g.type "
        "WHERE d.product_name = ? "
        "GROUP BY d.buyer_firm;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, product_name, -1, SQLITE_STATIC);

    printf("\n--- Покупатели товара '%s' ---\n", product_name);
    printf("%-25s %-15s %-15s\n", "Фирма-покупатель", "Кол-во", "Стоимость");
    printf("-------------------------------------------------------\n");

    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%-25s %-15d %-15.2f\n",
               (const char *)sqlite3_column_text(stmt, 0),
               sqlite3_column_int(stmt, 1),
               sqlite3_column_double(stmt, 2));
        found = 1;
    }
    if (!found) printf("Нет данных.\n");

    sqlite3_finalize(stmt);
    return 0;
}

int query_top_demand_type(sqlite3 *db) {
    const char *sql =
        "SELECT d.product_type, d.buyer_firm, SUM(d.quantity_sold) AS total_qty, "
        "SUM(d.quantity_sold * g.unit_price) AS total_cost "
        "FROM PB_DEALS d "
        "JOIN PB_GOODS g ON d.product_name = g.name AND d.product_type = g.type "
        "WHERE d.product_type = ("
        "    SELECT product_type FROM PB_DEALS "
        "    GROUP BY product_type ORDER BY SUM(quantity_sold) DESC LIMIT 1"
        ") "
        "GROUP BY d.buyer_firm "
        "ORDER BY total_qty DESC;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    printf("\n--- Вид товара с наибольшим спросом: продажи по покупателям ---\n");
    printf("%-20s %-25s %-12s %-15s\n", "Вид товара", "Покупатель", "Кол-во", "Стоимость");
    printf("----------------------------------------------------------------------\n");

    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%-20s %-25s %-12d %-15.2f\n",
               (const char *)sqlite3_column_text(stmt, 0),
               (const char *)sqlite3_column_text(stmt, 1),
               sqlite3_column_int(stmt, 2),
               sqlite3_column_double(stmt, 3));
        found = 1;
    }
    if (!found) printf("Нет данных.\n");

    sqlite3_finalize(stmt);
    return 0;
}

int query_top_broker(sqlite3 *db) {
    const char *sql =
        "SELECT b.id, b.surname, b.address, b.birth_year, "
        "GROUP_CONCAT(DISTINCT g.supplier_firm) AS suppliers, "
        "COUNT(d.id) AS deal_count "
        "FROM PB_BROKERS b "
        "JOIN PB_DEALS d ON b.id = d.broker_id "
        "JOIN PB_GOODS g ON d.product_name = g.name AND d.product_type = g.type "
        "WHERE b.id = ("
        "    SELECT broker_id FROM PB_DEALS "
        "    GROUP BY broker_id ORDER BY COUNT(*) DESC LIMIT 1"
        ") "
        "GROUP BY b.id;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    printf("\n--- Маклер с наибольшим количеством сделок ---\n");

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID: %d\n", sqlite3_column_int(stmt, 0));
        printf("Фамилия: %s\n", (const char *)sqlite3_column_text(stmt, 1));
        printf("Адрес: %s\n", (const char *)sqlite3_column_text(stmt, 2));
        printf("Год рождения: %d\n", sqlite3_column_int(stmt, 3));
        printf("Фирмы-поставщики: %s\n", (const char *)sqlite3_column_text(stmt, 4));
        printf("Количество сделок: %d\n", sqlite3_column_int(stmt, 5));
    } else {
        printf("Нет данных.\n");
    }

    sqlite3_finalize(stmt);
    return 0;
}

int query_supplier_brokers(sqlite3 *db) {
    const char *sql =
        "SELECT g.supplier_firm, b.surname, SUM(d.quantity_sold), "
        "SUM(d.quantity_sold * g.unit_price) "
        "FROM PB_DEALS d "
        "JOIN PB_GOODS g ON d.product_name = g.name AND d.product_type = g.type "
        "JOIN PB_BROKERS b ON d.broker_id = b.id "
        "GROUP BY g.supplier_firm, b.surname "
        "ORDER BY g.supplier_firm;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    printf("\n--- По каждой фирме-поставщику: маклеры и объемы продаж ---\n");
    printf("%-22s %-18s %-12s %-15s\n", "Поставщик", "Маклер", "Кол-во", "Стоимость");
    printf("-------------------------------------------------------------------\n");

    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%-22s %-18s %-12d %-15.2f\n",
               (const char *)sqlite3_column_text(stmt, 0),
               (const char *)sqlite3_column_text(stmt, 1),
               sqlite3_column_int(stmt, 2),
               sqlite3_column_double(stmt, 3));
        found = 1;
    }
    if (!found) printf("Нет данных.\n");

    sqlite3_finalize(stmt);
    return 0;
}
