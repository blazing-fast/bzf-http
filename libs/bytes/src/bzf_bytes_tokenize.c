#include "bzf_bytes.h"
#include <string.h>

bool bzf_bytes_tokenize(const struct bzf_bytes_immutable_view bytes, const struct bzf_bytes_immutable_view delimiter, size_t *offset, struct bzf_bytes_range* output_range) {
    assert(offset != NULL);
    assert(output_range != NULL);
    assert(delimiter.length != 0);
    assert(*offset <= bytes.length);

    while (*offset < bytes.length) {

        const struct bzf_bytes_immutable_view start = bzf_bytes_add_offset(bytes, *offset);

        const bzf_byte_t *found_first_byte = memchr(start.buffer, delimiter.buffer[0], start.length);
        if (found_first_byte == NULL) {
            return false;
        }

        const size_t index = (size_t)(found_first_byte - start.buffer);
        const size_t remaining_bytes_count = start.length - index - 1;

        if (remaining_bytes_count >= delimiter.length - 1 &&
            memcmp(found_first_byte + 1, delimiter.buffer + 1, delimiter.length - 1) == 0) {

            output_range->offset = *offset;
            output_range->length = index;

            *offset += index + delimiter.length;
            return true;
            }

        *offset += index + 1;
    }


    return false;
}
