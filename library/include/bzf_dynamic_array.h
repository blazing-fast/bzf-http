#ifndef TASKS_DYNAMIC_ARRAY_H
#define TASKS_DYNAMIC_ARRAY_H
#include <stddef.h>
#include <stdint.h>

#include "bzf_bytes.h"

struct bzf_dynamic_array;

enum bzf_dynamic_array_initialize_result{BZF_DYNAMIC_ARRAY_INITIALIZE_OK, BZF_DYNAMIC_ARRAY_INITIALIZE_MEMORY_ALLOCATION_ERROR};
enum bzf_dynamic_array_initialize_result bzf_dynamic_array_initialize(size_t element_size, size_t initial_capacity, struct bzf_dynamic_array** out);

void bzf_dynamic_array_get_ptr(const struct bzf_dynamic_array* dynamic_array, size_t index, void **out);

void bzf_dynamic_array_get(const struct bzf_dynamic_array* dynamic_array, size_t index, void *out);

void bzf_dynamic_array_set(const struct bzf_dynamic_array *dynamic_array, size_t index, const void *value);

enum bzf_dynamic_array_reserve_result { BZF_DYNAMIC_ARRAY_RESERVE_OK, BZF_DYNAMIC_ARRAY_RESERVE_MEMORY_ALLOCATION_ERROR };

enum bzf_dynamic_array_reserve_result bzf_dynamic_array_reserve(struct bzf_dynamic_array *dynamic_array, size_t new_min_capacity);

enum bzf_dynamic_array_push_back_result { BZF_DYNAMIC_ARRAY_PUSH_BACK_OK, BZF_DYNAMIC_ARRAY_PUSH_BACK_MEMORY_ALLOCATION_ERROR };

enum bzf_dynamic_array_push_back_result bzf_dynamic_array_push_back(struct bzf_dynamic_array *dynamic_array, const void *val);

void bzf_dynamic_array_get_back(const struct bzf_dynamic_array* dynamic_array, void *out);

#endif //TASKS_DYNAMIC_ARRAY_H