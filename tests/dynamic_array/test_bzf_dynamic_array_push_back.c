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

static void test_dynamic_array_push_and_get(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;
    bzf_dynamic_array_initialize(sizeof(int), 2, &arr);

    int val1 = 42;
    int val2 = 100;

    assert_int_equal(bzf_dynamic_array_push_back(arr, &val1), BZF_DYNAMIC_ARRAY_PUSH_BACK_OK);
    assert_int_equal(bzf_dynamic_array_push_back(arr, &val2), BZF_DYNAMIC_ARRAY_PUSH_BACK_OK);
    assert_int_equal(arr->number_of_elements, 2);

    int read1, read2;
    bzf_dynamic_array_get(arr, 0, &read1);
    bzf_dynamic_array_get(arr, 1, &read2);

    assert_int_equal(read1, 42);
    assert_int_equal(read2, 100);

    free(arr->buffer);
    free(arr);
}

static void test_dynamic_array_resize_on_push(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;
    bzf_dynamic_array_initialize(sizeof(int), 1, &arr);

    for (int i = 0; i < 10; i++) {
        int val = i * 10;
        assert_int_equal(bzf_dynamic_array_push_back(arr, &val), BZF_DYNAMIC_ARRAY_PUSH_BACK_OK);
    }

    assert_int_equal(arr->number_of_elements, 10);
    assert_true(arr->capacity >= 10);

    for (int i = 0; i < 10; i++) {
        int read;
        bzf_dynamic_array_get(arr, i, &read);
        assert_int_equal(read, i * 10);
    }

    free(arr->buffer);
    free(arr);
}

static void* malloc_always_fail(size_t size)
{
    (void)size;
    return NULL;
}

static void test_dynamic_array_push_back_malloc_fails(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;
    bzf_dynamic_array_initialize(sizeof(int), 1, &arr);

    int val = 42;
    assert_int_equal(bzf_dynamic_array_push_back(arr, &val), BZF_DYNAMIC_ARRAY_PUSH_BACK_OK);

    mock_bzf_os_malloc_set(malloc_always_fail);
    assert_int_equal(bzf_dynamic_array_push_back(arr, &val), BZF_DYNAMIC_ARRAY_PUSH_BACK_MEMORY_ALLOCATION_ERROR);
    mock_bzf_os_malloc_set(NULL);

    free(arr->buffer);
    free(arr);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_dynamic_array_push_and_get, per_test_setup),
        cmocka_unit_test_setup(test_dynamic_array_resize_on_push, per_test_setup),
        cmocka_unit_test_setup(test_dynamic_array_push_back_malloc_fails, per_test_setup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
