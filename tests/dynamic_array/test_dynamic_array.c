#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_dynamic_array.h"
#include <bzf_dynamic_array_impl.h>

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

static void test_dynamic_array_set(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;
    bzf_dynamic_array_initialize(sizeof(int), 2, &arr);

    int val = 1;
    bzf_dynamic_array_push_back(arr, &val);

    int updated = 999;
    bzf_dynamic_array_set(arr, 0, &updated);

    int read;
    bzf_dynamic_array_get(arr, 0, &read);
    assert_int_equal(read, 999);

    free(arr->buffer);
    free(arr);
}

static void test_dynamic_array_get_back(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;
    bzf_dynamic_array_initialize(sizeof(int), 2, &arr);

    int val1 = 10, val2 = 20;
    bzf_dynamic_array_push_back(arr, &val1);
    bzf_dynamic_array_push_back(arr, &val2);

    int back;
    bzf_dynamic_array_get_back(arr, &back);
    assert_int_equal(back, 20);

    free(arr->buffer);
    free(arr);
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

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dynamic_array_initialize),
        cmocka_unit_test(test_dynamic_array_push_and_get),
        cmocka_unit_test(test_dynamic_array_resize_on_push),
        cmocka_unit_test(test_dynamic_array_set),
        cmocka_unit_test(test_dynamic_array_get_back),
        cmocka_unit_test(test_dynamic_array_reserve),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
