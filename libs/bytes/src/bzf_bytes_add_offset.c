#include "bzf_bytes.h"

struct bzf_bytes_immutable_view bzf_bytes_add_offset(const struct bzf_bytes_immutable_view bytes, const size_t offset) {
    struct bzf_bytes_immutable_view res;
    res.buffer = bytes.buffer + offset;
    res.length = bytes.length - offset;

    return res;
}
