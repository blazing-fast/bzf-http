
#include "bzf_bytes.h"

#include <string.h>

bool bzf_bytes_equals(const struct bzf_bytes_immutable_view first, const struct bzf_bytes_immutable_view second) {
    return first.length == second.length &&
           memcmp(first.buffer, second.buffer, first.length) == 0;
}

struct bzf_bytes_immutable_view bzf_bytes_add_offset(const struct bzf_bytes_immutable_view bytes, const size_t offset) {
    struct bzf_bytes_immutable_view res;
    res.buffer = bytes.buffer + offset;
    res.length = bytes.length - offset;

    return res;
}

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

bool bzf_bytes_tokenize_once(const struct bzf_bytes_immutable_view bytes, const struct bzf_bytes_immutable_view delimiter, struct bzf_bytes_range *output_range) {
    size_t offset = 0;
    return bzf_bytes_tokenize(bytes, delimiter, &offset, output_range);
}

/**
 * Tokenize a byte buffer with a delimiter.
 * Supports the last token even if there is no trailing delimiter.
 *
 * @param input   The input buffer.
 * @param delimiter The delimiter to split on.
 * @param offset  Pointer to current offset. Will be updated.
 * @param token   Out parameter for the current token.
 * @return true if a token was produced, false if no more tokens.
 */
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
        // No more data
        return false;
    }

    // Slice starting from current offset
    struct bzf_bytes_immutable_view start = bzf_bytes_add_offset(input, *offset);
    const size_t remaining_to_compare = delimiter.length - 1;
    bzf_byte_t *found;

    while ((found = memchr(start.buffer, delimiter.buffer[0], start.length)) != NULL) {
        const size_t index = (size_t)(found - start.buffer);
        const size_t remaining = start.length - index - 1;

        if (remaining >= remaining_to_compare &&
            memcmp(found + 1, delimiter.buffer + 1, remaining_to_compare) == 0) {
            // Found delimiter
            token->buffer = start.buffer;
            token->length = index;
            *offset += index + delimiter.length;
            return true;
            }

        // Delimiter not matching fully, skip first occurrence
        start = bzf_bytes_add_offset(start, index + 1);
    }

    // No delimiter found — last token is remainder
    token->buffer = start.buffer;
    token->length = start.length;
    *offset = input.length;
    return true;
}


/**
 * Split a bytes_t buffer into tokens (zero-copy, like PHP explode).
 * Last element receives the remainder if max_tokens is reached or
 * if no trailing delimiter exists.
 *
 * @param input       The input buffer to split.
 * @param delimiter   The byte sequence to split on.
 * @param out_ranges
 * @param max_tokens  Maximum number of tokens to write.
 * @return Number of tokens written (<= max_tokens)
 */
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

