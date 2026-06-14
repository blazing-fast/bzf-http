#include "../include/bzf_dynamic_array.h"
#include <assert.h>
#include <string.h>
#include "bzf_dynamic_array_impl.h"

void bzf_dynamic_array_set(const struct bzf_dynamic_array *dynamic_array, const size_t index, const void *value) {
    assert(index < dynamic_array->number_of_elements);
    assert(value != NULL);
    void *const destination = dynamic_array->buffer + index * dynamic_array->element_size;
    memcpy(destination, value, dynamic_array->element_size);
}
