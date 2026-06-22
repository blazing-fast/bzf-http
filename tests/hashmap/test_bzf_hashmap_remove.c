#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_hashmap.h"

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

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_hashmap_remove),
        cmocka_unit_test(test_hashmap_remove_not_found),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
