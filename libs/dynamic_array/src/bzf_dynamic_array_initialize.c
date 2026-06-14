#include "../include/bzf_dynamic_array.h"
#include <assert.h>
#include <stdlib.h>
#include "bzf_dynamic_array_impl.h"
#include "bzf_os.h"

enum bzf_dynamic_array_initialize_result bzf_dynamic_array_initialize(const size_t element_size, const size_t initial_capacity,
                                                                      struct bzf_dynamic_array **const out) {
    struct bzf_dynamic_array *dynamic_array = bzf_os_malloc(sizeof(struct bzf_dynamic_array));
    if (dynamic_array == NULL) {
        return BZF_DYNAMIC_ARRAY_INITIALIZE_MEMORY_ALLOCATION_ERROR;
    }
    dynamic_array->element_size = element_size;
    dynamic_array->number_of_elements = 0;
    dynamic_array->capacity = initial_capacity;
    dynamic_array->buffer = bzf_os_malloc(element_size * initial_capacity);
    if (dynamic_array->buffer == NULL) {
        free(dynamic_array);
        return BZF_DYNAMIC_ARRAY_INITIALIZE_MEMORY_ALLOCATION_ERROR;
    }
    *out = dynamic_array;
    return BZF_DYNAMIC_ARRAY_INITIALIZE_OK;
}
