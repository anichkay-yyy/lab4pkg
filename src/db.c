#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int db_open(const char *path, sqlite3 **db) {
    int rc = sqlite3_open(path, db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    db_execute(*db, "PRAGMA foreign_keys = ON;");
    return SQLITE_OK;
}

int db_close(sqlite3 *db) {
    return sqlite3_close(db);
}

int db_execute(sqlite3 *db, const char *sql) {
    char *err_msg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    return rc;
}

int db_init(sqlite3 *db) {
    const char *schema =
        "PRAGMA foreign_keys = ON;"

        "CREATE TABLE IF NOT EXISTS PB_BROKERS ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    surname TEXT NOT NULL,"
        "    address TEXT NOT NULL,"
        "    birth_year INTEGER NOT NULL CHECK(birth_year > 1900 AND birth_year < 2010)"
        ");"

        "CREATE TABLE IF NOT EXISTS PB_GOODS ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    name TEXT NOT NULL,"
        "    type TEXT NOT NULL,"
        "    unit_price REAL NOT NULL CHECK(unit_price > 0),"
        "    supplier_firm TEXT NOT NULL,"
        "    expiration_date TEXT NOT NULL,"
        "    quantity_supplied INTEGER NOT NULL CHECK(quantity_supplied >= 0)"
        ");"

        "CREATE TABLE IF NOT EXISTS PB_DEALS ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    deal_date TEXT NOT NULL,"
        "    product_name TEXT NOT NULL,"
        "    product_type TEXT NOT NULL,"
        "    quantity_sold INTEGER NOT NULL CHECK(quantity_sold > 0),"
        "    broker_id INTEGER NOT NULL REFERENCES PB_BROKERS(id),"
        "    buyer_firm TEXT NOT NULL"
        ");"

        "CREATE TABLE IF NOT EXISTS PB_BROKER_STATS ("
        "    broker_id INTEGER PRIMARY KEY REFERENCES PB_BROKERS(id),"
        "    total_quantity_sold INTEGER NOT NULL DEFAULT 0,"
        "    total_deal_sum REAL NOT NULL DEFAULT 0.0"
        ");"

        "CREATE TABLE IF NOT EXISTS PB_USERS ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    username TEXT NOT NULL UNIQUE,"
        "    password_hash TEXT NOT NULL,"
        "    role TEXT NOT NULL CHECK(role IN ('admin', 'broker')),"
        "    broker_id INTEGER REFERENCES PB_BROKERS(id)"
        ");";

    return db_execute(db, schema);
}
