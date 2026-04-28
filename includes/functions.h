#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sqlite3.h>

/* Функция 4: обновление статистики маклера при совершении сделки (*) */
int fn_update_broker_stats(sqlite3 *db, int deal_id);

/* Функция 5: обновление данных о товаре на указанную дату, удаление сделок */
int fn_consolidate_goods(sqlite3 *db, const char *period_date);

/* Функция 6: вывод информации о сделках маклера на указанную дату (*) */
int fn_broker_deals_on_date(sqlite3 *db, const char *date, int broker_id);

#endif
