#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "db.h"

static sqlite3 *test_db = NULL;

static int suite_init(void) {
    return db_open(":memory:", &test_db);
}

static int suite_clean(void) {
    return db_close(test_db);
}

static void test_db_open_close(void) {
    sqlite3 *db2 = NULL;
    CU_ASSERT_EQUAL(db_open(":memory:", &db2), SQLITE_OK);
    CU_ASSERT_PTR_NOT_NULL(db2);
    CU_ASSERT_EQUAL(db_close(db2), SQLITE_OK);
}

static void test_db_init_creates_tables(void) {
    CU_ASSERT_EQUAL(db_init(test_db), SQLITE_OK);

    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name LIKE 'PB_%';";
    CU_ASSERT_EQUAL(sqlite3_prepare_v2(test_db, sql, -1, &stmt, NULL), SQLITE_OK);
    CU_ASSERT_EQUAL(sqlite3_step(stmt), SQLITE_ROW);
    int count = sqlite3_column_int(stmt, 0);
    CU_ASSERT_EQUAL(count, 5);
    sqlite3_finalize(stmt);
}

static void test_db_foreign_keys_enabled(void) {
    db_init(test_db);
    sqlite3_stmt *stmt;
    const char *sql = "PRAGMA foreign_keys;";
    CU_ASSERT_EQUAL(sqlite3_prepare_v2(test_db, sql, -1, &stmt, NULL), SQLITE_OK);
    CU_ASSERT_EQUAL(sqlite3_step(stmt), SQLITE_ROW);
    int fk = sqlite3_column_int(stmt, 0);
    CU_ASSERT_EQUAL(fk, 1);
    sqlite3_finalize(stmt);
}

static void test_db_execute_invalid_sql(void) {
    int rc = db_execute(test_db, "INVALID SQL STATEMENT;");
    CU_ASSERT_NOT_EQUAL(rc, SQLITE_OK);
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("DB Tests", suite_init, suite_clean);

    CU_add_test(suite, "test_db_open_close", test_db_open_close);
    CU_add_test(suite, "test_db_init_creates_tables", test_db_init_creates_tables);
    CU_add_test(suite, "test_db_foreign_keys_enabled", test_db_foreign_keys_enabled);
    CU_add_test(suite, "test_db_execute_invalid_sql", test_db_execute_invalid_sql);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int failures = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return failures > 0 ? 1 : 0;
}
