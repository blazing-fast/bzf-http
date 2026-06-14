#include "bzf_http_server.h"

void coucouEndpoint(struct bzf_http_route_output* output) {
    static const unsigned char response[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 6\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Coucou";
    struct bzf_bytes_mutable_buffer mutable_buffer ={(bzf_byte_t*)response, sizeof(response)-1, sizeof(response)-1 };
    *output->response = mutable_buffer;
}
