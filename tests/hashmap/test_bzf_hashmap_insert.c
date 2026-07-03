#include <bzf_os_mocks.h>
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

void* malloc_always_fail(size_t size)
{
    (void)size;
    return NULL;
}

static void test_hashmap_return_error_on_malloc_error(void **state)
{
    (void)state;
    struct bzf_hashmap *map = NULL;
    const enum bzf_hashmap_initialize_result bzf_hasmap_initialiaze_result = bzf_hashmap_initialize(&map);
    assert_int_equal(bzf_hasmap_initialiaze_result, BZF_HASHMAP_INITIALIZE_OK);
    mock_bzf_os_malloc_set(malloc_always_fail);
    bzf_byte_t key[] = "hello";
    struct bzf_bytes_immutable_view kv = {key, 5};

    int value = 42;
    assert_int_equal(bzf_hashmap_insert(map, kv, &value), BZF_HASHMAP_INSERT_ALLOCATION_ERROR);
    mock_bzf_os_malloc_set(NULL);
    bzf_hashmap_free(map, NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_hashmap_insert_and_get),
        cmocka_unit_test(test_hashmap_return_error_on_malloc_error)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
