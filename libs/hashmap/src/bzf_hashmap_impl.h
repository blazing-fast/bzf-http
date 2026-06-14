#ifndef TASKS_BZF_HASHMAP_IMPL_H
#define TASKS_BZF_HASHMAP_IMPL_H
#include <stdint.h>

#include "bzf_bytes.h"

struct bzf_hashmap_node {
    struct bzf_bytes_immutable_view key;
    void* data;
    struct bzf_hashmap_node* next;
};

struct bzf_hashmap {
    uint_fast32_t number_of_elements;
    uint_fast32_t capacity;
    struct bzf_hashmap_node** elements;
};
#endif //TASKS_BZF_HASHMAP_IMPL_H