#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_hashmap.h"
#include "bzf_hashmap_mocks.h"

static int per_test_setup(void **state)
{
    (void)state;
    mock_bzf_hashmap_hash_string_set(NULL);
    return 0;
}

static uint32_t hash_always_zero(struct bzf_bytes_immutable_view key)
{
    (void)key;
    return 0;
}

static void test_hashmap_remove(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    const enum bzf_hashmap_initialize_result bzf_hashmap_initialize_result = bzf_hashmap_initialize(&map);
    assert_int_equal(bzf_hashmap_initialize_result, BZF_HASHMAP_INITIALIZE_OK);

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
    const enum bzf_hashmap_initialize_result bzf_hashmap_initialize_result = bzf_hashmap_initialize(&map);
    assert_int_equal(bzf_hashmap_initialize_result, BZF_HASHMAP_INITIALIZE_OK);

    bzf_byte_t key[] = "not_here";
    struct bzf_bytes_immutable_view kv = {key, 8};

    void *data = NULL;
    assert_int_equal(bzf_hashmap_remove(map, kv, &data), BZF_HASHMAP_REMOVE_NOT_FOUND);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_remove_null_data(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    const enum bzf_hashmap_initialize_result res = bzf_hashmap_initialize(&map);
    assert_int_equal(res, BZF_HASHMAP_INITIALIZE_OK);

    bzf_byte_t key[] = "null_data";
    struct bzf_bytes_immutable_view kv = {key, 9};
    int value = 42;

    assert_int_equal(bzf_hashmap_insert(map, kv, &value), BZF_HASHMAP_INSERT_OK);
    assert_int_equal(bzf_hashmap_remove(map, kv, NULL), BZF_HASHMAP_REMOVE_OK);

    void *data = NULL;
    assert_int_equal(bzf_hashmap_get(map, kv, &data), BZF_HASHMAP_GET_NOT_FOUND);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_remove_multiple_entries(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    assert_int_equal(bzf_hashmap_initialize(&map), BZF_HASHMAP_INITIALIZE_OK);

    bzf_byte_t k1[] = "alpha";
    bzf_byte_t k2[] = "beta";
    bzf_byte_t k3[] = "gamma";
    int v1 = 1, v2 = 2, v3 = 3;

    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k1, 5}, &v1), BZF_HASHMAP_INSERT_OK);
    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k2, 4}, &v2), BZF_HASHMAP_INSERT_OK);
    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k3, 5}, &v3), BZF_HASHMAP_INSERT_OK);

    void *data = NULL;
    assert_int_equal(bzf_hashmap_remove(map, (struct bzf_bytes_immutable_view){k2, 4}, &data), BZF_HASHMAP_REMOVE_OK);
    assert_int_equal(*(int *)data, 2);

    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k1, 5}, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 1);
    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k3, 5}, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 3);
    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k2, 4}, &data), BZF_HASHMAP_GET_NOT_FOUND);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_remove_twice(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    assert_int_equal(bzf_hashmap_initialize(&map), BZF_HASHMAP_INITIALIZE_OK);

    bzf_byte_t key[] = "double";
    struct bzf_bytes_immutable_view kv = {key, 6};
    int value = 111;

    assert_int_equal(bzf_hashmap_insert(map, kv, &value), BZF_HASHMAP_INSERT_OK);

    void *data = NULL;
    assert_int_equal(bzf_hashmap_remove(map, kv, &data), BZF_HASHMAP_REMOVE_OK);
    assert_int_equal(*(int *)data, 111);

    assert_int_equal(bzf_hashmap_remove(map, kv, &data), BZF_HASHMAP_REMOVE_NOT_FOUND);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_remove_head_of_chain(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    assert_int_equal(bzf_hashmap_initialize(&map), BZF_HASHMAP_INITIALIZE_OK);

    mock_bzf_hashmap_hash_string_set(hash_always_zero);

    bzf_byte_t k1[] = "first";
    bzf_byte_t k2[] = "second";
    int v1 = 10, v2 = 20;

    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k1, 5}, &v1), BZF_HASHMAP_INSERT_OK);
    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k2, 6}, &v2), BZF_HASHMAP_INSERT_OK);

    void *data = NULL;
    assert_int_equal(bzf_hashmap_remove(map, (struct bzf_bytes_immutable_view){k2, 6}, &data), BZF_HASHMAP_REMOVE_OK);
    assert_int_equal(*(int *)data, 20);

    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k1, 5}, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 10);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_remove_tail_of_chain(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    assert_int_equal(bzf_hashmap_initialize(&map), BZF_HASHMAP_INITIALIZE_OK);

    mock_bzf_hashmap_hash_string_set(hash_always_zero);

    bzf_byte_t k1[] = "first";
    bzf_byte_t k2[] = "second";
    int v1 = 10, v2 = 20;

    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k1, 5}, &v1), BZF_HASHMAP_INSERT_OK);
    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k2, 6}, &v2), BZF_HASHMAP_INSERT_OK);

    void *data = NULL;
    assert_int_equal(bzf_hashmap_remove(map, (struct bzf_bytes_immutable_view){k1, 5}, &data), BZF_HASHMAP_REMOVE_OK);
    assert_int_equal(*(int *)data, 10);

    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k2, 6}, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 20);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_remove_middle_of_chain(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    assert_int_equal(bzf_hashmap_initialize(&map), BZF_HASHMAP_INITIALIZE_OK);

    mock_bzf_hashmap_hash_string_set(hash_always_zero);

    bzf_byte_t k1[] = "first";
    bzf_byte_t k2[] = "second";
    bzf_byte_t k3[] = "third";
    int v1 = 1, v2 = 2, v3 = 3;

    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k1, 5}, &v1), BZF_HASHMAP_INSERT_OK);
    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k2, 6}, &v2), BZF_HASHMAP_INSERT_OK);
    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k3, 5}, &v3), BZF_HASHMAP_INSERT_OK);

    void *data = NULL;
    assert_int_equal(bzf_hashmap_remove(map, (struct bzf_bytes_immutable_view){k2, 6}, &data), BZF_HASHMAP_REMOVE_OK);
    assert_int_equal(*(int *)data, 2);

    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k3, 5}, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 3);
    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k1, 5}, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 1);
    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k2, 6}, &data), BZF_HASHMAP_GET_NOT_FOUND);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_remove_not_found_single_in_bucket(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    assert_int_equal(bzf_hashmap_initialize(&map), BZF_HASHMAP_INITIALIZE_OK);

    mock_bzf_hashmap_hash_string_set(hash_always_zero);

    bzf_byte_t k1[] = "first";
    bzf_byte_t k2[] = "missing";
    int v1 = 1;

    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k1, 5}, &v1), BZF_HASHMAP_INSERT_OK);

    void *data = NULL;
    assert_int_equal(bzf_hashmap_remove(map, (struct bzf_bytes_immutable_view){k2, 7}, &data), BZF_HASHMAP_REMOVE_NOT_FOUND);

    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k1, 5}, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 1);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_remove_not_found_in_non_empty_bucket(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    assert_int_equal(bzf_hashmap_initialize(&map), BZF_HASHMAP_INITIALIZE_OK);

    mock_bzf_hashmap_hash_string_set(hash_always_zero);

    bzf_byte_t k1[] = "first";
    bzf_byte_t k2[] = "second";
    bzf_byte_t k3[] = "missing";
    int v1 = 1, v2 = 2;

    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k1, 5}, &v1), BZF_HASHMAP_INSERT_OK);
    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k2, 6}, &v2), BZF_HASHMAP_INSERT_OK);

    void *data = NULL;
    assert_int_equal(bzf_hashmap_remove(map, (struct bzf_bytes_immutable_view){k3, 7}, &data), BZF_HASHMAP_REMOVE_NOT_FOUND);

    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k1, 5}, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 1);
    assert_int_equal(bzf_hashmap_get(map, (struct bzf_bytes_immutable_view){k2, 6}, &data), BZF_HASHMAP_GET_FOUND);
    assert_int_equal(*(int *)data, 2);

    bzf_hashmap_free(map, NULL);
}

static void test_hashmap_remove_duplicate_key(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    assert_int_equal(bzf_hashmap_initialize(&map), BZF_HASHMAP_INITIALIZE_OK);

    bzf_byte_t key[] = "dup";
    struct bzf_bytes_immutable_view kv = {key, 3};
    int v1 = 100, v2 = 200;

    assert_int_equal(bzf_hashmap_insert(map, kv, &v1), BZF_HASHMAP_INSERT_OK);
    assert_int_equal(bzf_hashmap_insert(map, kv, &v2), BZF_HASHMAP_INSERT_OK);

    void *data = NULL;
    assert_int_equal(bzf_hashmap_remove(map, kv, &data), BZF_HASHMAP_REMOVE_OK);
    assert_int_equal(*(int *)data, 200);

    assert_int_equal(bzf_hashmap_remove(map, kv, &data), BZF_HASHMAP_REMOVE_OK);
    assert_int_equal(*(int *)data, 100);

    assert_int_equal(bzf_hashmap_remove(map, kv, &data), BZF_HASHMAP_REMOVE_NOT_FOUND);

    bzf_hashmap_free(map, NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_hashmap_remove, per_test_setup),
        cmocka_unit_test_setup(test_hashmap_remove_not_found, per_test_setup),
        cmocka_unit_test_setup(test_hashmap_remove_null_data, per_test_setup),
        cmocka_unit_test_setup(test_hashmap_remove_multiple_entries, per_test_setup),
        cmocka_unit_test_setup(test_hashmap_remove_twice, per_test_setup),
        cmocka_unit_test_setup(test_hashmap_remove_head_of_chain, per_test_setup),
        cmocka_unit_test_setup(test_hashmap_remove_tail_of_chain, per_test_setup),
        cmocka_unit_test_setup(test_hashmap_remove_middle_of_chain, per_test_setup),
        cmocka_unit_test_setup(test_hashmap_remove_not_found_single_in_bucket, per_test_setup),
        cmocka_unit_test_setup(test_hashmap_remove_not_found_in_non_empty_bucket, per_test_setup),
        cmocka_unit_test_setup(test_hashmap_remove_duplicate_key, per_test_setup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
