//
// Created by aym on 08.05.2026.
//

#ifndef TASKS_BZF_DYNAMIC_ARRAY_IMPL_H
#define TASKS_BZF_DYNAMIC_ARRAY_IMPL_H
#include "bzf_bytes.h"

struct bzf_dynamic_array {
    size_t element_size;
    size_t number_of_elements;
    size_t capacity;
    bzf_byte_t* buffer;
};

#endif //TASKS_BZF_DYNAMIC_ARRAY_IMPL_H