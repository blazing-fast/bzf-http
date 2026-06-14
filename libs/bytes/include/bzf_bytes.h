#ifndef TASKS_BZF_BYTES_H
#define TASKS_BZF_BYTES_H
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned char bzf_byte_t;

struct bzf_bytes_immutable_view{
    const bzf_byte_t *buffer;
    size_t length;
};

struct bzf_bytes_mutable_view {
    bzf_byte_t* buffer;
    size_t length;
};

struct bzf_bytes_mutable_buffer {
    bzf_byte_t *buffer;
    size_t length;
    size_t capacity;
};

struct bzf_bytes_range {
    size_t offset;
    size_t length;
};

static inline struct bzf_bytes_immutable_view bzf_bytes_immutable_view_from_immutable_view_and_range(const struct bzf_bytes_immutable_view view, const struct bzf_bytes_range range) {
    return (struct bzf_bytes_immutable_view){view.buffer + range.offset, range.length};
}

static inline struct bzf_bytes_immutable_view bzf_bytes_immutable_view_from_mutable_buffer_and_range(const struct bzf_bytes_mutable_buffer mutable_buffer, const struct bzf_bytes_range range) {
    return (struct bzf_bytes_immutable_view){mutable_buffer.buffer + range.offset, range.length};
}

static inline struct bzf_bytes_mutable_view bzf_bytes_mutable_view_from_mutable_buffer_and_range(const struct bzf_bytes_mutable_buffer mutable_buffer, const struct bzf_bytes_range range) {
    return (struct bzf_bytes_mutable_view){mutable_buffer.buffer + range.offset, range.length};
}

static inline struct bzf_bytes_mutable_view bzf_bytes_mutable_view_from_mutable_view_and_range(const struct bzf_bytes_mutable_view mutable_view, const struct bzf_bytes_range range) {
    return (struct bzf_bytes_mutable_view){mutable_view.buffer + range.offset, range.length};
}

static inline struct bzf_bytes_immutable_view bzf_bytes_immutable_view_from_mutable_view(const struct bzf_bytes_mutable_view mutable_view) {
    return (struct bzf_bytes_immutable_view){mutable_view.buffer, mutable_view.length};
}

static inline struct bzf_bytes_immutable_view bzf_bytes_immutable_view_from_mutable_buffer(const struct bzf_bytes_mutable_buffer mutable_buffer){
    return (struct bzf_bytes_immutable_view){mutable_buffer.buffer, mutable_buffer.length};
}


/**
 *
 * @param t
 * @return the immutable view
 * @deprecated shitty name
 */
static inline struct bzf_bytes_immutable_view bytes_buffer_to_byte_view(const struct bzf_bytes_mutable_buffer t) {
    return (struct bzf_bytes_immutable_view){t.buffer, t.length};
}

bool bzf_bytes_equals(struct bzf_bytes_immutable_view first, struct bzf_bytes_immutable_view second);

struct bzf_bytes_immutable_view bzf_bytes_add_offset(const struct bzf_bytes_immutable_view bytes, const size_t offset);

bool bzf_bytes_tokenize(
    struct bzf_bytes_immutable_view bytes,
    struct bzf_bytes_immutable_view delimiter,
    size_t *offset,
    struct bzf_bytes_range *output_range
);

bool bzf_bytes_tokenize_once(struct bzf_bytes_immutable_view bytes, struct bzf_bytes_immutable_view delimiter, struct bzf_bytes_range *token);


bool bzf_bytes_tokenize_with_remainder(
    struct bzf_bytes_immutable_view input,
    struct bzf_bytes_immutable_view delimiter,
    size_t *offset,
    struct bzf_bytes_immutable_view *token
);


size_t bzf_bytes_split(struct bzf_bytes_immutable_view input, struct bzf_bytes_immutable_view delimiter,
                   struct bzf_bytes_range* out_ranges, size_t max_tokens);

static inline struct bzf_bytes_range bytes_left_trim(const struct bzf_bytes_immutable_view v) {
    size_t current_index = 0;
    while (current_index < v.length && isspace(v.buffer[current_index])) {
        current_index++;
    }
    return (struct bzf_bytes_range){current_index, v.length - current_index};
}

static inline void bzf_bytes_lower(const struct bzf_bytes_mutable_view* const mutable_view) {
    assert(mutable_view != NULL);
    bzf_byte_t* p = mutable_view->buffer;
    for (const bzf_byte_t* end = mutable_view->buffer + mutable_view->length; p < end; ++p) {
        *p = (bzf_byte_t)tolower(*p);
    }
}
#endif //TASKS_BZF_BYTES_H