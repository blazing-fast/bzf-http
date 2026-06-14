#include "../include/bzf_dynamic_array.h"
#include <assert.h>
#include <string.h>
#include "bzf_dynamic_array_impl.h"

enum bzf_dynamic_array_push_back_result bzf_dynamic_array_push_back(struct bzf_dynamic_array *dynamic_array, const void *val) {

    assert(dynamic_array != NULL);
    assert(val != NULL);

    if (dynamic_array->number_of_elements == dynamic_array->capacity) {
        const enum bzf_dynamic_array_reserve_result dynamic_array_reserve_result = bzf_dynamic_array_reserve(
            dynamic_array, dynamic_array->capacity * 2);
        if (dynamic_array_reserve_result == BZF_DYNAMIC_ARRAY_RESERVE_MEMORY_ALLOCATION_ERROR) {
            return BZF_DYNAMIC_ARRAY_PUSH_BACK_MEMORY_ALLOCATION_ERROR;
        }
    }

    memcpy(&dynamic_array->buffer[dynamic_array->number_of_elements * dynamic_array->element_size], val,
           dynamic_array->element_size);
    dynamic_array->number_of_elements++;
    return BZF_DYNAMIC_ARRAY_PUSH_BACK_OK;
}
