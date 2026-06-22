#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_hashmap.h"

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

static void test_hashmap_get_multiple_entries(void **state) {
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

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_hashmap_get_not_found),
        cmocka_unit_test(test_hashmap_get_multiple_entries),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
