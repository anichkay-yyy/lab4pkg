#ifndef GOODS_H
#define GOODS_H

#include <sqlite3.h>

int goods_insert(sqlite3 *db, const char *name, const char *type, double unit_price,
                 const char *supplier_firm, const char *expiration_date, int quantity_supplied);
int goods_update(sqlite3 *db, int id, const char *name, const char *type, double unit_price,
                 const char *supplier_firm, const char *expiration_date, int quantity_supplied);
int goods_delete(sqlite3 *db, int id);
int goods_list(sqlite3 *db);
int goods_count(sqlite3 *db);

#endif
