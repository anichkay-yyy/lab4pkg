#ifndef DEAL_H
#define DEAL_H

#include <sqlite3.h>

int deal_insert(sqlite3 *db, const char *deal_date, const char *product_name,
                const char *product_type, int quantity_sold, int broker_id, const char *buyer_firm);
int deal_update(sqlite3 *db, int id, const char *deal_date, const char *product_name,
                const char *product_type, int quantity_sold, int broker_id, const char *buyer_firm);
int deal_delete(sqlite3 *db, int id);
int deal_list(sqlite3 *db, int broker_id);
int deal_count(sqlite3 *db);

#endif
