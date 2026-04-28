#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "db.h"
#include "broker.h"
#include "goods.h"
#include "deal.h"
#include "queries.h"

static sqlite3 *test_db = NULL;

static int suite_init(void) {
    if (db_open(":memory:", &test_db) != SQLITE_OK) return -1;
    if (db_init(test_db) != SQLITE_OK) return -1;

    int b1 = broker_insert(test_db, "Иванов", "г. Минск", 1985);
    int b2 = broker_insert(test_db, "Петров", "г. Брест", 1990);

    goods_insert(test_db, "Chanel", "духи", 100.0, "LuxFirm", "2027-01-01", 500);
    goods_insert(test_db, "Dior", "туалетная вода", 80.0, "FrenchCo", "2027-06-01", 300);

    deal_insert(test_db, "2025-01-15", "Chanel", "духи", 50, b1, "Buyer1");
    deal_insert(test_db, "2025-02-20", "Chanel", "духи", 30, b2, "Buyer2");
    deal_insert(test_db, "2025-03-10", "Dior", "туалетная вода", 40, b1, "Buyer1");
    deal_insert(test_db, "2025-03-15", "Dior", "туалетная вода", 20, b1, "Buyer3");

    return 0;
}

static int suite_clean(void) {
    return db_close(test_db);
}

static void test_query_product_sales(void) {
    int rc = query_product_sales_by_period(test_db, "Chanel", "2025-01-01", "2025-12-31");
    CU_ASSERT_EQUAL(rc, 0);
}

static void test_query_product_buyers(void) {
    int rc = query_product_buyers(test_db, "Chanel");
    CU_ASSERT_EQUAL(rc, 0);
}

static void test_query_top_demand(void) {
    int rc = query_top_demand_type(test_db);
    CU_ASSERT_EQUAL(rc, 0);
}

static void test_query_top_broker(void) {
    int rc = query_top_broker(test_db);
    CU_ASSERT_EQUAL(rc, 0);
}

static void test_query_supplier_brokers(void) {
    int rc = query_supplier_brokers(test_db);
    CU_ASSERT_EQUAL(rc, 0);
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Queries Tests", suite_init, suite_clean);

    CU_add_test(suite, "test_query_product_sales", test_query_product_sales);
    CU_add_test(suite, "test_query_product_buyers", test_query_product_buyers);
    CU_add_test(suite, "test_query_top_demand", test_query_top_demand);
    CU_add_test(suite, "test_query_top_broker", test_query_top_broker);
    CU_add_test(suite, "test_query_supplier_brokers", test_query_supplier_brokers);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int failures = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return failures > 0 ? 1 : 0;
}
