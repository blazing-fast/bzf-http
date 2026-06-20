#include "../include/bzf_dynamic_array.h"
#include <assert.h>
#include "bzf_dynamic_array_impl.h"

void bzf_dynamic_array_get_ptr(const struct bzf_dynamic_array *dynamic_array, const size_t index, void **out) {
    assert(index < dynamic_array->number_of_elements);
    assert(out != NULL);
    *out = dynamic_array->buffer + index * dynamic_array->element_size;
}
