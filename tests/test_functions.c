#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "db.h"
#include "broker.h"
#include "goods.h"
#include "deal.h"
#include "functions.h"

static sqlite3 *test_db = NULL;
static int test_broker_id = 0;

static int suite_init(void) {
    if (db_open(":memory:", &test_db) != SQLITE_OK) return -1;
    if (db_init(test_db) != SQLITE_OK) return -1;

    test_broker_id = broker_insert(test_db, "ТестМаклер", "г. Минск", 1985);
    goods_insert(test_db, "TestParfum", "духи", 100.0, "TestFirm", "2027-01-01", 500);

    return 0;
}

static int suite_clean(void) {
    return db_close(test_db);
}

static void test_fn_update_broker_stats(void) {
    int deal_id = deal_insert(test_db, "2025-05-01", "TestParfum", "духи", 25, test_broker_id, "TestBuyer");
    CU_ASSERT(deal_id > 0);

    int rc = fn_update_broker_stats(test_db, deal_id);
    CU_ASSERT_EQUAL(rc, 0);

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(test_db,
        "SELECT total_quantity_sold, total_deal_sum FROM PB_BROKER_STATS WHERE broker_id = ?;",
        -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, test_broker_id);

    CU_ASSERT_EQUAL(sqlite3_step(stmt), SQLITE_ROW);
    CU_ASSERT_EQUAL(sqlite3_column_int(stmt, 0), 25);
    CU_ASSERT_DOUBLE_EQUAL(sqlite3_column_double(stmt, 1), 2500.0, 0.01);
    sqlite3_finalize(stmt);
}

static void test_fn_consolidate_goods(void) {
    deal_insert(test_db, "2025-01-10", "TestParfum", "духи", 10, test_broker_id, "Buyer1");
    deal_insert(test_db, "2025-01-15", "TestParfum", "духи", 15, test_broker_id, "Buyer2");

    int rc = fn_consolidate_goods(test_db, "2025-01-20");
    CU_ASSERT_EQUAL(rc, 0);

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(test_db,
        "SELECT COUNT(*) FROM PB_DEALS WHERE deal_date <= '2025-01-20';",
        -1, &stmt, NULL);
    CU_ASSERT_EQUAL(sqlite3_step(stmt), SQLITE_ROW);
    CU_ASSERT_EQUAL(sqlite3_column_int(stmt, 0), 0);
    sqlite3_finalize(stmt);
}

static void test_fn_broker_deals_on_date(void) {
    deal_insert(test_db, "2025-06-01", "TestParfum", "духи", 5, test_broker_id, "DateBuyer");

    int rc = fn_broker_deals_on_date(test_db, "2025-06-01", test_broker_id);
    CU_ASSERT_EQUAL(rc, 0);
}

static void test_fn_update_stats_invalid_deal(void) {
    int rc = fn_update_broker_stats(test_db, 99999);
    CU_ASSERT_NOT_EQUAL(rc, 0);
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Functions Tests", suite_init, suite_clean);

    CU_add_test(suite, "test_fn_update_broker_stats", test_fn_update_broker_stats);
    CU_add_test(suite, "test_fn_consolidate_goods", test_fn_consolidate_goods);
    CU_add_test(suite, "test_fn_broker_deals_on_date", test_fn_broker_deals_on_date);
    CU_add_test(suite, "test_fn_update_stats_invalid_deal", test_fn_update_stats_invalid_deal);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int failures = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return failures > 0 ? 1 : 0;
}
