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

static void test_dynamic_array_initialize(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;

    enum bzf_dynamic_array_initialize_result res =
        bzf_dynamic_array_initialize(sizeof(int), 4, &arr);

    assert_int_equal(res, BZF_DYNAMIC_ARRAY_INITIALIZE_OK);
    assert_non_null(arr);
    assert_int_equal(arr->number_of_elements, 0);
    assert_int_equal(arr->capacity, 4);
    assert_int_equal(arr->element_size, sizeof(int));

    free(arr->buffer);
    free(arr);
}

static void* malloc_always_fail(size_t size)
{
    (void)size;
    return NULL;
}

static void test_dynamic_array_initialize_malloc_fails(void **state) {
    (void)state;
    mock_bzf_os_malloc_set(malloc_always_fail);
    struct bzf_dynamic_array *arr = NULL;

    enum bzf_dynamic_array_initialize_result res =
        bzf_dynamic_array_initialize(sizeof(int), 4, &arr);

    assert_int_equal(res, BZF_DYNAMIC_ARRAY_INITIALIZE_MEMORY_ALLOCATION_ERROR);
    assert_null(arr);

    mock_bzf_os_malloc_set(NULL);
}

static int malloc_call_count = 0;

static void* malloc_fail_on_second_call(size_t size)
{
    malloc_call_count++;
    if (malloc_call_count == 2) {
        return NULL;
    }
    return malloc(size);
}

static int free_count = 0;

static void counting_free(void* ptr)
{
    (void)ptr;
    free_count++;
    free(ptr);
}

static void test_dynamic_array_initialize_buffer_malloc_fails(void **state) {
    (void)state;
    malloc_call_count = 0;
    free_count = 0;
    mock_bzf_os_malloc_set(malloc_fail_on_second_call);
    mock_bzf_os_free_set(counting_free);
    struct bzf_dynamic_array *arr = NULL;

    enum bzf_dynamic_array_initialize_result res =
        bzf_dynamic_array_initialize(sizeof(int), 4, &arr);

    assert_int_equal(res, BZF_DYNAMIC_ARRAY_INITIALIZE_MEMORY_ALLOCATION_ERROR);
    assert_null(arr);
    assert_int_equal(free_count, 1);

    mock_bzf_os_malloc_set(NULL);
    mock_bzf_os_free_set(NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_dynamic_array_initialize, per_test_setup),
        cmocka_unit_test_setup(test_dynamic_array_initialize_malloc_fails, per_test_setup),
        cmocka_unit_test_setup(test_dynamic_array_initialize_buffer_malloc_fails, per_test_setup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
