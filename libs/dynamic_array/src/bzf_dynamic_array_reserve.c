#include "../include/bzf_dynamic_array.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "bzf_dynamic_array_impl.h"
#include "bzf_os.h"

enum bzf_dynamic_array_reserve_result bzf_dynamic_array_reserve(struct bzf_dynamic_array *dynamic_array, const size_t new_min_capacity) {
    if (dynamic_array->capacity >= new_min_capacity) {
        return BZF_DYNAMIC_ARRAY_RESERVE_OK;
    }

    void *new_buffer = bzf_os_malloc(new_min_capacity * dynamic_array->element_size);
    if (new_buffer == NULL) {
        return BZF_DYNAMIC_ARRAY_RESERVE_MEMORY_ALLOCATION_ERROR;
    }

    memcpy(new_buffer, dynamic_array->buffer, dynamic_array->capacity * dynamic_array->element_size);

    bzf_os_free(dynamic_array->buffer);

    dynamic_array->buffer = new_buffer;
    dynamic_array->capacity = new_min_capacity;


    return BZF_DYNAMIC_ARRAY_RESERVE_OK;
}
