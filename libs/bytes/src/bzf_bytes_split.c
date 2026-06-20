#include "bzf_bytes.h"

size_t bzf_bytes_split(const struct bzf_bytes_immutable_view input, const struct bzf_bytes_immutable_view delimiter,
                   struct bzf_bytes_range *out_ranges, const size_t max_tokens) {
    assert(out_ranges != NULL);
    assert(max_tokens > 0);
    assert(input.buffer != NULL);
    assert(delimiter.buffer != NULL && delimiter.length > 0);

    size_t token_count = 0;
    size_t offset = 0;
    const size_t last_index = max_tokens - 1;

    for (size_t i = 0; i < last_index; ++i) {
        if (!bzf_bytes_tokenize(input, delimiter, &offset, &out_ranges[token_count])) {
            return token_count;
        }
        token_count++;
    }

    if (offset < input.length) {
        out_ranges[token_count].offset = offset;
        out_ranges[token_count].length = input.length - offset;
        token_count++;
    }

    return token_count;
}
