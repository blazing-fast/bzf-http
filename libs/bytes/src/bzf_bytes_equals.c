#include "bzf_bytes.h"
#include <string.h>

bool bzf_bytes_equals(const struct bzf_bytes_immutable_view first, const struct bzf_bytes_immutable_view second) {
    return first.length == second.length &&
           memcmp(first.buffer, second.buffer, first.length) == 0;
}
