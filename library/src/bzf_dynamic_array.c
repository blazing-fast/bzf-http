
#include "../include/bzf_dynamic_array.h"
#include "bzf_dynamic_array_impl.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "bzf_bytes.h"

enum bzf_dynamic_array_initialize_result bzf_dynamic_array_initialize(const size_t element_size, const size_t initial_capacity,
                                                           struct bzf_dynamic_array **out) {
    assert(out != NULL);

    *out = malloc(sizeof(struct bzf_dynamic_array));

    if (*out == NULL) {
        return BZF_DYNAMIC_ARRAY_INITIALIZE_MEMORY_ALLOCATION_ERROR;
    }

    bzf_byte_t *buffer = malloc(element_size * initial_capacity);
    if (buffer == NULL) {
        return BZF_DYNAMIC_ARRAY_INITIALIZE_MEMORY_ALLOCATION_ERROR;
    }

    struct bzf_dynamic_array * const dynamic_array = *out;
    dynamic_array->number_of_elements = 0;
    dynamic_array->element_size = element_size;
    dynamic_array->capacity = initial_capacity;
    dynamic_array->buffer = buffer;


    return BZF_DYNAMIC_ARRAY_INITIALIZE_OK;
}



void bzf_dynamic_array_get_ptr(const struct bzf_dynamic_array* dynamic_array, const size_t index, void **out) {
    assert(dynamic_array != NULL);
    assert(index < dynamic_array->number_of_elements);
    *out = &dynamic_array->buffer[index * dynamic_array->element_size];
}

void bzf_dynamic_array_get(const struct bzf_dynamic_array* dynamic_array, const size_t index, void *out) {
    assert(dynamic_array != NULL);
    assert(index < dynamic_array->number_of_elements);
    const bzf_byte_t *src = &dynamic_array->buffer[index * dynamic_array->element_size];
    memcpy(out, src, dynamic_array->element_size);
}

void bzf_dynamic_array_set(const struct bzf_dynamic_array *dynamic_array, const size_t index, const void *value) {
    assert(dynamic_array != NULL);
    assert(index < dynamic_array->number_of_elements);
    bzf_byte_t *dest = &dynamic_array->buffer[index * dynamic_array->element_size];
    memcpy(dest, value, dynamic_array->element_size);
}

enum bzf_dynamic_array_reserve_result bzf_dynamic_array_reserve(struct bzf_dynamic_array *dynamic_array, const size_t new_min_capacity) {
    if (dynamic_array->capacity >= new_min_capacity) {
        return BZF_DYNAMIC_ARRAY_RESERVE_OK;
    }


    void *new_buffer = malloc(new_min_capacity * dynamic_array->element_size);
    if (new_buffer == NULL) {
        return BZF_DYNAMIC_ARRAY_RESERVE_MEMORY_ALLOCATION_ERROR;
    }


    memcpy(new_buffer, dynamic_array->buffer, dynamic_array->capacity * dynamic_array->element_size);

    free(dynamic_array->buffer);

    dynamic_array->buffer = new_buffer;
    dynamic_array->capacity = new_min_capacity;


    return BZF_DYNAMIC_ARRAY_RESERVE_OK;
}

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


void bzf_dynamic_array_get_back(const struct bzf_dynamic_array* dynamic_array, void *out) {
    assert(dynamic_array != NULL);
    assert(out != NULL);
    bzf_dynamic_array_get(dynamic_array, dynamic_array->number_of_elements - 1, out);
}
