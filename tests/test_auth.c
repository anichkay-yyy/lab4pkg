#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "db.h"
#include "auth.h"
#include <string.h>

static sqlite3 *test_db = NULL;

static int suite_init(void) {
    if (db_open(":memory:", &test_db) != SQLITE_OK) return -1;
    if (db_init(test_db) != SQLITE_OK) return -1;
    auth_register(test_db, "testadmin", "admin123", "admin", 0);
    return 0;
}

static int suite_clean(void) {
    return db_close(test_db);
}

static void test_hash_consistency(void) {
    unsigned long h1 = hash_password("test123");
    unsigned long h2 = hash_password("test123");
    CU_ASSERT_EQUAL(h1, h2);
}

static void test_hash_different_passwords(void) {
    unsigned long h1 = hash_password("password1");
    unsigned long h2 = hash_password("password2");
    CU_ASSERT_NOT_EQUAL(h1, h2);
}

static void test_login_success(void) {
    User user;
    int rc = auth_login(test_db, "testadmin", "admin123", &user);
    CU_ASSERT_EQUAL(rc, 0);
    CU_ASSERT_STRING_EQUAL(user.username, "testadmin");
    CU_ASSERT_STRING_EQUAL(user.role, "admin");
}

static void test_login_wrong_password(void) {
    User user;
    int rc = auth_login(test_db, "testadmin", "wrongpassword", &user);
    CU_ASSERT_NOT_EQUAL(rc, 0);
}

static void test_login_nonexistent_user(void) {
    User user;
    int rc = auth_login(test_db, "nobody", "password", &user);
    CU_ASSERT_NOT_EQUAL(rc, 0);
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Auth Tests", suite_init, suite_clean);

    CU_add_test(suite, "test_hash_consistency", test_hash_consistency);
    CU_add_test(suite, "test_hash_different_passwords", test_hash_different_passwords);
    CU_add_test(suite, "test_login_success", test_login_success);
    CU_add_test(suite, "test_login_wrong_password", test_login_wrong_password);
    CU_add_test(suite, "test_login_nonexistent_user", test_login_nonexistent_user);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int failures = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return failures > 0 ? 1 : 0;
}
