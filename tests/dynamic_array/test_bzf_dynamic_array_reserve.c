#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_dynamic_array.h"
#include <bzf_dynamic_array_impl.h>
#include <bzf_os_mocks.h>

static int per_test_setup(void **state)
{
    (void)state;
    mock_bzf_os_malloc_set(NULL);
    return 0;
}

static void test_dynamic_array_reserve(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;
    bzf_dynamic_array_initialize(sizeof(int), 2, &arr);

    assert_int_equal(bzf_dynamic_array_reserve(arr, 10), BZF_DYNAMIC_ARRAY_RESERVE_OK);
    assert_int_equal(arr->capacity, 10);

    assert_int_equal(bzf_dynamic_array_reserve(arr, 5), BZF_DYNAMIC_ARRAY_RESERVE_OK);

    free(arr->buffer);
    free(arr);
}

static void* malloc_always_fail(size_t size)
{
    (void)size;
    return NULL;
}

static void test_dynamic_array_reserve_malloc_fails(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;
    bzf_dynamic_array_initialize(sizeof(int), 2, &arr);

    mock_bzf_os_malloc_set(malloc_always_fail);
    assert_int_equal(bzf_dynamic_array_reserve(arr, 10), BZF_DYNAMIC_ARRAY_RESERVE_MEMORY_ALLOCATION_ERROR);
    assert_int_equal(arr->capacity, 2);
    mock_bzf_os_malloc_set(NULL);

    free(arr->buffer);
    free(arr);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_dynamic_array_reserve, per_test_setup),
        cmocka_unit_test_setup(test_dynamic_array_reserve_malloc_fails, per_test_setup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
