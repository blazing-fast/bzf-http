#include <bzf_os_mocks.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_hashmap.h"

static int per_test_setup(void **state)
{
    (void)state;
    mock_bzf_os_malloc_set(NULL);
    mock_bzf_os_calloc_set(NULL);
    mock_bzf_os_free_set(NULL);
    return 0;
}

static void test_hashmap_initialize(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;

    enum bzf_hashmap_initialize_result res = bzf_hashmap_initialize(&map);
    assert_int_equal(res, BZF_HASHMAP_INITIALIZE_OK);
    assert_non_null(map);

    bzf_hashmap_free(map, NULL);
}

static void* malloc_always_fail(size_t size)
{
    (void)size;
    return NULL;
}

static void* calloc_always_fail(size_t nmemb, size_t size)
{
    (void)nmemb;
    (void)size;
    return NULL;
}

static void test_hashmap_return_error_when_malloc_fails(void **state)
{
    (void)state;
    mock_bzf_os_malloc_set(malloc_always_fail);
    struct bzf_hashmap *map = NULL;
    const enum bzf_hashmap_initialize_result res = bzf_hashmap_initialize(&map);
    assert_int_equal(res, BZF_HASHMAP_INITIALIZE_ALLOCATION_ERROR);
    mock_bzf_os_malloc_set(NULL);
}

static int free_call_count = 0;

static void counting_free(void* ptr)
{
    free_call_count++;
    free(ptr);
}

static void test_hashmap_error_when_calloc_fails(void **state)
{
    (void)state;
    free_call_count = 0;

    mock_bzf_os_calloc_set(calloc_always_fail);
    mock_bzf_os_free_set(counting_free);

    struct bzf_hashmap *map = NULL;
    enum bzf_hashmap_initialize_result res = bzf_hashmap_initialize(&map);
    assert_int_equal(res, BZF_HASHMAP_INITIALIZE_ALLOCATION_ERROR);
    assert_int_equal(free_call_count, 1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_hashmap_initialize, per_test_setup),
        cmocka_unit_test_setup(test_hashmap_return_error_when_malloc_fails, per_test_setup),
        cmocka_unit_test_setup(test_hashmap_error_when_calloc_fails, per_test_setup)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
