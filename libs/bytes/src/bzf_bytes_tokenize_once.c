#include "bzf_bytes.h"

bool bzf_bytes_tokenize_once(const struct bzf_bytes_immutable_view bytes, const struct bzf_bytes_immutable_view delimiter, struct bzf_bytes_range *output_range) {
    size_t offset = 0;
    return bzf_bytes_tokenize(bytes, delimiter, &offset, output_range);
}
