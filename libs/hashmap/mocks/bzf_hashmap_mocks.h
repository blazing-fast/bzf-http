#ifndef BZF_HASHMAP_MOCKS_H
#define BZF_HASHMAP_MOCKS_H

#include "bzf_hashmap.h"

void mock_bzf_hashmap_insert_set(
    enum bzf_hashmap_insert_result (*fn)(
        struct bzf_hashmap* const,
        const struct bzf_bytes_immutable_view,
        void*));

#endif
