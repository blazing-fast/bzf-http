#include <stdio.h>
#include <unistd.h>

#include "bzf_http_server.h"


void coucouEndpoint(struct bzf_http_route_output* output) {
    static const unsigned char response[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 6\r\n"   // "Coucou" = 6 bytes
    "Connection: close\r\n"
    "\r\n"
    "Coucou";
    struct bzf_bytes_mutable_buffer mutable_buffer ={(bzf_byte_t*)response, sizeof(response)-1, sizeof(response)-1 };
    *output->response = mutable_buffer;
}

void rootEndpoint(struct bzf_http_route_output* output) {
    static const unsigned char response[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 4\r\n"   // "root" = 4 bytes
    "Connection: close\r\n"
    "\r\n"
    "Root";
    struct bzf_bytes_mutable_buffer mutable_buffer ={(bzf_byte_t*)response, sizeof(response)-1, sizeof(response)-1 };
    *output->response = mutable_buffer;
}


int main(void) {
    // ok
    struct bzf_http_server http_server;
    enum bzf_http_server_initialize_result http_server_initialize_result = bzf_http_server_initialize(&http_server);
    if (http_server_initialize_result == BZF_HTTP_SERVER_INITIALIZE_MEMORY_ALLOCATION_ERROR) {
        fprintf(stderr, "memoy allocation error");
        // todo handle
        return 42;
    }

    // ok
    struct bzf_http_route_handler coucou_handler = {{(const bzf_byte_t*)"/coucou", 7}, coucouEndpoint};
    bzf_http_add_route_handler(&http_server, coucou_handler);
    struct bzf_http_route_handler root_handler = {{(const bzf_byte_t*)"/", 1}, rootEndpoint};
    bzf_http_add_route_handler(&http_server, root_handler);
    struct bzf_http_client client;
    // pas ok

    // todo ne pas passer null ptetre
    bzf_http_server_accept_client(&http_server, &client, NULL);
    bzf_http_handle_request(client, http_server.handlers);
    fflush(stdout);
    bzf_http_server_destroy(&http_server);
    return 0;
}
