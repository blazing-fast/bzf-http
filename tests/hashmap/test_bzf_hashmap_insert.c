#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_hashmap.h"

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

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_hashmap_insert_and_get),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
