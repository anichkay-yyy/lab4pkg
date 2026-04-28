#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "db.h"
#include "goods.h"

static sqlite3 *test_db = NULL;

static int suite_init(void) {
    if (db_open(":memory:", &test_db) != SQLITE_OK) return -1;
    return db_init(test_db);
}

static int suite_clean(void) {
    return db_close(test_db);
}

static void test_goods_insert(void) {
    int id = goods_insert(test_db, "TestParfum", "духи", 50.0, "TestFirm", "2027-01-01", 100);
    CU_ASSERT(id > 0);
}

static void test_goods_count(void) {
    int before = goods_count(test_db);
    goods_insert(test_db, "TestParfum2", "туалетная вода", 30.0, "Firm2", "2027-06-01", 200);
    int after = goods_count(test_db);
    CU_ASSERT_EQUAL(after, before + 1);
}

static void test_goods_insert_negative_price(void) {
    int id = goods_insert(test_db, "Bad", "духи", -10.0, "Firm", "2027-01-01", 100);
    CU_ASSERT_EQUAL(id, -1);
}

static void test_goods_delete(void) {
    int id = goods_insert(test_db, "ToDelete", "духи", 10.0, "Firm", "2027-01-01", 50);
    int before = goods_count(test_db);
    goods_delete(test_db, id);
    int after = goods_count(test_db);
    CU_ASSERT_EQUAL(after, before - 1);
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Goods Tests", suite_init, suite_clean);

    CU_add_test(suite, "test_goods_insert", test_goods_insert);
    CU_add_test(suite, "test_goods_count", test_goods_count);
    CU_add_test(suite, "test_goods_insert_negative_price", test_goods_insert_negative_price);
    CU_add_test(suite, "test_goods_delete", test_goods_delete);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int failures = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return failures > 0 ? 1 : 0;
}
