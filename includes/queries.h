#ifndef QUERIES_H
#define QUERIES_H

#include <sqlite3.h>

/* По каждому названию товара - сведения о проданном количестве и общей стоимости за указанный период */
int query_product_sales_by_period(sqlite3 *db, const char *product_name, const char *date_from, const char *date_to);

/* По каждому названию товара - перечень фирм-покупателей с кол-вом и стоимостью */
int query_product_buyers(sqlite3 *db, const char *product_name);

/* По виду товара с наибольшим спросом - кол-во и стоимость по каждой фирме-покупателю (*) */
int query_top_demand_type(sqlite3 *db);

/* По маклеру с макс. кол-вом сделок - сведения о нем и фирмах-поставщиках */
int query_top_broker(sqlite3 *db);

/* По каждой фирме-поставщику - список маклеров с кол-вом и стоимостью */
int query_supplier_brokers(sqlite3 *db);

#endif
