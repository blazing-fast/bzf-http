#include "../include/bzf_dynamic_array.h"
#include <assert.h>
#include <string.h>
#include "bzf_dynamic_array_impl.h"

void bzf_dynamic_array_get(const struct bzf_dynamic_array *dynamic_array, const size_t index, void *out) {
    assert(index < dynamic_array->number_of_elements);
    assert(out != NULL);
    const void *const input = dynamic_array->buffer + index * dynamic_array->element_size;
    memcpy(out, input, dynamic_array->element_size);
}
