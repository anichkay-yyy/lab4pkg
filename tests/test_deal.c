#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "db.h"
#include "broker.h"
#include "goods.h"
#include "deal.h"

static sqlite3 *test_db = NULL;
static int test_broker_id = 0;

static int suite_init(void) {
    if (db_open(":memory:", &test_db) != SQLITE_OK) return -1;
    if (db_init(test_db) != SQLITE_OK) return -1;
    test_broker_id = broker_insert(test_db, "ТестМаклер", "г. Минск", 1985);
    goods_insert(test_db, "TestGoods", "духи", 100.0, "TestFirm", "2027-01-01", 500);
    return 0;
}

static int suite_clean(void) {
    return db_close(test_db);
}

static void test_deal_insert(void) {
    int id = deal_insert(test_db, "2025-01-01", "TestGoods", "духи", 10, test_broker_id, "BuyerFirm");
    CU_ASSERT(id > 0);
}

static void test_deal_count(void) {
    int before = deal_count(test_db);
    deal_insert(test_db, "2025-02-01", "TestGoods", "духи", 5, test_broker_id, "BuyerFirm2");
    int after = deal_count(test_db);
    CU_ASSERT_EQUAL(after, before + 1);
}

static void test_deal_insert_invalid_broker(void) {
    int id = deal_insert(test_db, "2025-03-01", "TestGoods", "духи", 5, 9999, "BuyerFirm");
    CU_ASSERT_EQUAL(id, -1);
}

static void test_deal_delete(void) {
    int id = deal_insert(test_db, "2025-04-01", "TestGoods", "духи", 3, test_broker_id, "DelBuyer");
    int before = deal_count(test_db);
    deal_delete(test_db, id);
    int after = deal_count(test_db);
    CU_ASSERT_EQUAL(after, before - 1);
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Deal Tests", suite_init, suite_clean);

    CU_add_test(suite, "test_deal_insert", test_deal_insert);
    CU_add_test(suite, "test_deal_count", test_deal_count);
    CU_add_test(suite, "test_deal_insert_invalid_broker", test_deal_insert_invalid_broker);
    CU_add_test(suite, "test_deal_delete", test_deal_delete);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int failures = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return failures > 0 ? 1 : 0;
}
