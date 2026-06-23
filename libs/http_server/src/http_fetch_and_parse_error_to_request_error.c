#include "private.h"
#include "http.h"
#include <stdio.h>

enum bzf_handle_request_result http_fetch_and_parse_error_to_request_error(
    const enum http_fetch_and_parse_head_result http_fetch_and_parse_head_result)
{
    switch (http_fetch_and_parse_head_result)
    {
    case HTTP_FETCH_AND_PARSE_HEAD_CONNECTION_CLOSED:
        return BZF_HANDLE_REQUEST_CLOSED;
    case HTTP_FETCH_AND_PARSE_HEAD_HEADERS_TOO_LARGE:
    case HTTP_FETCH_AND_PARSE_HEAD_INVALID_HEADER:
    case HTTP_FETCH_AND_PARSE_HEAD_INVALID_REQUEST_LINE:
        return BZF_HANDLE_REQUEST_INVALID;
    case HTTP_FETCH_AND_PARSE_HEAD_MEMORY_ALLOCATION_FAILED:
        return BZF_HANDLE_REQUEST_MEMORY_ALLOCATION_ERROR;
    case HTTP_FETCH_AND_PARSE_HEAD_UNEXPECTED_ERROR:
        return BZF_HANDLE_REQUEST_UNEXPECTED_ERROR;
    default:
        fprintf(stderr, "Unhandled enum value : %d", (int)http_fetch_and_parse_head_result);
        return BZF_HANDLE_REQUEST_UNEXPECTED_ERROR;
    }
}
