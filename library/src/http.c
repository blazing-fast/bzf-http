
#include "http.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "bzf_bytes.h"


bool parse_http_request_line(const struct bzf_bytes_immutable_view request_line, struct http_request_line* out)
{
    assert(out != NULL);
    assert(request_line.buffer != NULL);

    enum
    {
        HTTP_PART_METHOD,
        HTTP_PART_TARGET,
        HTTP_PART_VERSION,
        HTTP_PART_COUNT
    };

    static const struct bzf_bytes_immutable_view space_byte_view = {(const bzf_byte_t*)" ", 1};

    struct bzf_bytes_range line_parts[HTTP_PART_COUNT];
    if (bzf_bytes_split(request_line, space_byte_view, line_parts, HTTP_PART_COUNT) != HTTP_PART_COUNT)
    {
        return false;
    }

    out->method = line_parts[HTTP_PART_METHOD];
    out->target = line_parts[HTTP_PART_TARGET];
    out->version = line_parts[HTTP_PART_VERSION];

    return true;
}

enum http_fetch_and_parse_head_result http_fetch_and_parse_head(const int client_fd,
                                                             struct bzf_bytes_mutable_buffer *http_head_buffer,
                                                             struct http_fetch_and_parse_head_output *output) {
    assert(http_head_buffer != NULL);
    assert(output != NULL);

    bool headers_done = false;
    bool is_first_line = true;

    size_t parsed_index = 0;
    struct bzf_bytes_range bytes_range;


    struct bzf_bytes_immutable_view headers_buffer_view = bytes_buffer_to_byte_view(*http_head_buffer);


    enum {
        HTTP_RANGE_KEY,
        HTTP_RANGE_VALUE,
        HTTP_RANGE_COUNT
    };

    struct bzf_bytes_range http_line_key_value[HTTP_RANGE_COUNT];

    enum { READ_BUFFER_SIZE = 1024 };
    unsigned char read_buf[READ_BUFFER_SIZE];

    while (!headers_done) {
        ssize_t n = recv(client_fd, read_buf, READ_BUFFER_SIZE, 0);

        if (n == 0) {
            return HTTP_FETCH_AND_PARSE_HEAD_CONNECTION_CLOSED;
        }

        if (n < 0) {
            return HTTP_FETCH_AND_PARSE_HEAD_UNEXPECTED_ERROR;
        }

        if (http_head_buffer->length + n > http_head_buffer->capacity) {
            return HTTP_FETCH_AND_PARSE_HEAD_HEADERS_TOO_LARGE;
        }

        memcpy(http_head_buffer->buffer + http_head_buffer->length, read_buf, n);
        http_head_buffer->length += n;
        headers_buffer_view.length += n;

        static const struct bzf_bytes_immutable_view http_separator = {
            (const bzf_byte_t *) "\r\n",
            2
        };

        while (bzf_bytes_tokenize(headers_buffer_view, http_separator, &parsed_index, &bytes_range) && bytes_range.length > 0) {
            const struct bzf_bytes_immutable_view line_immutable_view =
                    bzf_bytes_immutable_view_from_mutable_buffer_and_range(
                        *http_head_buffer, bytes_range);
            const struct bzf_bytes_mutable_view line_mutable_view = bzf_bytes_mutable_view_from_mutable_buffer_and_range(
                *http_head_buffer, bytes_range);
            if (is_first_line) {
                if (!parse_http_request_line(line_immutable_view, &output->request_line)) {
                    return HTTP_FETCH_AND_PARSE_HEAD_INVALID_REQUEST_LINE;
                }
                is_first_line = false;
            } else {
                static const struct bzf_bytes_immutable_view colon_byte_view = {(const bzf_byte_t *) ":", 1};
                const size_t number_of_elements = bzf_bytes_split(line_immutable_view, colon_byte_view,
                                                              http_line_key_value, HTTP_RANGE_COUNT);
                if (number_of_elements != 2) {
                    // header is invalid, doesn't contains ":"
                    return HTTP_FETCH_AND_PARSE_HEAD_INVALID_HEADER;
                }
                // todo better handle of multiple fucking malloc
                struct bzf_bytes_immutable_view *value_immutable_view = malloc(sizeof(struct bzf_bytes_immutable_view));
                if (value_immutable_view == NULL) {
                    return HTTP_FETCH_AND_PARSE_HEAD_MEMORY_ALLOCATION_FAILED;
                }

                const struct bzf_bytes_immutable_view immutable_value_view =
                        bzf_bytes_immutable_view_from_immutable_view_and_range(
                            line_immutable_view, http_line_key_value[HTTP_RANGE_VALUE]);


                *value_immutable_view = bzf_bytes_immutable_view_from_immutable_view_and_range(
                    immutable_value_view, bytes_left_trim(immutable_value_view));

                struct bzf_bytes_mutable_view key_mutable_view = bzf_bytes_mutable_view_from_mutable_view_and_range(
                    line_mutable_view, http_line_key_value[HTTP_RANGE_KEY]);

                bzf_bytes_lower(&key_mutable_view);

                bzf_hashmap_insert(&output->headers,
                               bzf_bytes_immutable_view_from_mutable_view(key_mutable_view), value_immutable_view);
            }
        }


        if (strstr((char *) http_head_buffer->buffer, "\r\n\r\n")) {
            headers_done = true;
        }
    }
    return HTTP_FETCH_AND_PARSE_HEAD_OK;
}

