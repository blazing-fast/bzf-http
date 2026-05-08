

#ifndef TASKS_HTTP_H
#define TASKS_HTTP_H
#include "bzf_bytes.h"
#include "../include/bzf_hashmap.h"
#include "utilities.h"


struct http_request_line {
   struct bzf_bytes_range method;
   struct bzf_bytes_range target;
   struct bzf_bytes_range version;
};
bool parse_http_request_line(struct bzf_bytes_immutable_view request_line, struct http_request_line* out);



enum http_fetch_and_parse_head_result {
   HTTP_FETCH_AND_PARSE_HEAD_OK,
   HTTP_FETCH_AND_PARSE_HEAD_CONNECTION_CLOSED,
   HTTP_FETCH_AND_PARSE_HEAD_UNEXPECTED_ERROR,
   HTTP_FETCH_AND_PARSE_HEAD_HEADERS_TOO_LARGE,
   HTTP_FETCH_AND_PARSE_HEAD_INVALID_HEADER,
   HTTP_FETCH_AND_PARSE_HEAD_MEMORY_ALLOCATION_FAILED,
   HTTP_FETCH_AND_PARSE_HEAD_INVALID_REQUEST_LINE
};

struct http_fetch_and_parse_head_output {
   struct http_request_line request_line;
   struct bzf_hashmap headers;
};

enum http_fetch_and_parse_head_result http_fetch_and_parse_head(int client_fd, struct bzf_bytes_mutable_buffer* http_head_buffer, struct http_fetch_and_parse_head_output* output);

#endif //TASKS_HTTP_H