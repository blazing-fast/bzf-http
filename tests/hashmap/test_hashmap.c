#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_hashmap.h"

static void test_hashmap_initialize(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;

    enum bzf_hashmap_initialize_result res = bzf_hashmap_initialize(&map);
    assert_int_equal(res, BZF_HASHMAP_INITIALIZE_OK);
    assert_non_null(map);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_insert_and_get(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    bzf_hashmap_initialize(&map);

    bzf_byte_t key[] = "hello";
    struct bzf_bytes_immutable_view kv = {key, 5};

    int value = 42;
    assert_int_equal(bzf_hashmap_insert(map, kv, &value), BZF_HASHMAP_INSERT_OK);

    void *data = NULL;
    assert_int_equal(bzf_hashmap_get(map, kv, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 42);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_get_not_found(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    bzf_hashmap_initialize(&map);

    bzf_byte_t key[] = "missing";
    struct bzf_bytes_immutable_view kv = {key, 7};

    void *data = NULL;
    assert_int_equal(bzf_hashmap_get(map, kv, &data), BZF_HASHMAP_GET_NOT_FOUND);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_multiple_entries(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    bzf_hashmap_initialize(&map);

    bzf_byte_t k1[] = "a";
    bzf_byte_t k2[] = "b";
    bzf_byte_t k3[] = "c";

    int v1 = 1, v2 = 2, v3 = 3;

    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k1, 1}, &v1), BZF_HASHMAP_INSERT_OK);
    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k2, 1}, &v2), BZF_HASHMAP_INSERT_OK);
    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k3, 1}, &v3), BZF_HASHMAP_INSERT_OK);

    void *data = NULL;
    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k1, 1}, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 1);

    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k2, 1}, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 2);

    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k3, 1}, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 3);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_remove(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    bzf_hashmap_initialize(&map);

    bzf_byte_t key[] = "remove_me";
    struct bzf_bytes_immutable_view kv = {key, 9};

    int value = 777;
    assert_int_equal(bzf_hashmap_insert(map, kv, &value), BZF_HASHMAP_INSERT_OK);

    void *data = NULL;
    assert_int_equal(bzf_hashmap_remove(map, kv, &data), BZF_HASHMAP_REMOVE_OK);
    assert_int_equal(*(int *)data, 777);

    assert_int_equal(bzf_hashmap_get(map, kv, &data), BZF_HASHMAP_GET_NOT_FOUND);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_remove_not_found(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    bzf_hashmap_initialize(&map);

    bzf_byte_t key[] = "not_here";
    struct bzf_bytes_immutable_view kv = {key, 8};

    void *data = NULL;
    assert_int_equal(bzf_hashmap_remove(map, kv, &data), BZF_HASHMAP_REMOVE_NOT_FOUND);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_hash_string_consistency(void **state) {
    (void)state;
    bzf_byte_t key1[] = "consistent";
    bzf_byte_t key2[] = "consistent";

    struct bzf_bytes_immutable_view v1 = {key1, 10};
    struct bzf_bytes_immutable_view v2 = {key2, 10};

    assert_int_equal(bzf_hashmap_hash_string(v1), bzf_hashmap_hash_string(v2));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_hashmap_initialize),
        cmocka_unit_test(test_hashmap_insert_and_get),
        cmocka_unit_test(test_hashmap_get_not_found),
        cmocka_unit_test(test_hashmap_multiple_entries),
        cmocka_unit_test(test_hashmap_remove),
        cmocka_unit_test(test_hashmap_remove_not_found),
        cmocka_unit_test(test_hashmap_hash_string_consistency),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
