#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "db.h"
#include "broker.h"

static sqlite3 *test_db = NULL;

static int suite_init(void) {
    if (db_open(":memory:", &test_db) != SQLITE_OK) return -1;
    return db_init(test_db);
}

static int suite_clean(void) {
    return db_close(test_db);
}

static void test_broker_insert(void) {
    int id = broker_insert(test_db, "Тестов", "г. Минск", 1990);
    CU_ASSERT(id > 0);
}

static void test_broker_count(void) {
    int before = broker_count(test_db);
    broker_insert(test_db, "Новиков", "г. Брест", 1985);
    int after = broker_count(test_db);
    CU_ASSERT_EQUAL(after, before + 1);
}

static void test_broker_update(void) {
    int id = broker_insert(test_db, "Старый", "г. Гомель", 1980);
    CU_ASSERT(id > 0);
    int rc = broker_update(test_db, id, "Новый", "г. Витебск", 1981);
    CU_ASSERT_EQUAL(rc, 0);
}

static void test_broker_delete(void) {
    int id = broker_insert(test_db, "Удаляемый", "г. Могилев", 1975);
    int before = broker_count(test_db);
    broker_delete(test_db, id);
    int after = broker_count(test_db);
    CU_ASSERT_EQUAL(after, before - 1);
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Broker Tests", suite_init, suite_clean);

    CU_add_test(suite, "test_broker_insert", test_broker_insert);
    CU_add_test(suite, "test_broker_count", test_broker_count);
    CU_add_test(suite, "test_broker_update", test_broker_update);
    CU_add_test(suite, "test_broker_delete", test_broker_delete);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int failures = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return failures > 0 ? 1 : 0;
}
