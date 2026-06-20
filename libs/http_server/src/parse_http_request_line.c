#include "http.h"
#include <assert.h>
#include <stdbool.h>
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
