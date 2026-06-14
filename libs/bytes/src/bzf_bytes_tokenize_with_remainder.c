#include "bzf_bytes.h"
#include <string.h>

bool bzf_bytes_tokenize_with_remainder(
    const struct bzf_bytes_immutable_view input,
    const struct bzf_bytes_immutable_view delimiter,
    size_t *offset,
    struct bzf_bytes_immutable_view *token
) {
    assert(offset != NULL);
    assert(token != NULL);
    assert(delimiter.length > 0);
    assert(*offset <= input.length);

    if (*offset >= input.length) {
        return false;
    }

    struct bzf_bytes_immutable_view start = bzf_bytes_add_offset(input, *offset);
    const size_t remaining_to_compare = delimiter.length - 1;
    bzf_byte_t *found;

    while ((found = memchr(start.buffer, delimiter.buffer[0], start.length)) != NULL) {
        const size_t index = (size_t)(found - start.buffer);
        const size_t remaining = start.length - index - 1;

        if (remaining >= remaining_to_compare &&
            memcmp(found + 1, delimiter.buffer + 1, remaining_to_compare) == 0) {
            token->buffer = start.buffer;
            token->length = index;
            *offset += index + delimiter.length;
            return true;
            }

        start = bzf_bytes_add_offset(start, index + 1);
    }

    token->buffer = start.buffer;
    token->length = start.length;
    *offset = input.length;
    return true;
}
