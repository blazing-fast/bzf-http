#include "../include/bzf_dynamic_array.h"
#include <assert.h>
#include <string.h>
#include "bzf_dynamic_array_impl.h"

void bzf_dynamic_array_get_back(const struct bzf_dynamic_array *dynamic_array, void *out) {
    assert(dynamic_array->number_of_elements > 0);
    assert(out != NULL);
    const void *const input = dynamic_array->buffer + (dynamic_array->number_of_elements - 1) * dynamic_array->element_size;
    memcpy(out, input, dynamic_array->element_size);
}
