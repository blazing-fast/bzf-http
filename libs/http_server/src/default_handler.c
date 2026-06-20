#include "bzf_http_server.h"

void defaultHandler(struct bzf_http_route_output* output) {
    static const unsigned char response[] =
    "HTTP/1.1 404 NOT FOUND\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 9\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Not found";
    const struct bzf_bytes_mutable_buffer mutable_buffer ={(bzf_byte_t*)response, sizeof(response)-1, sizeof(response)-1 };
    *output->response = mutable_buffer;
}
