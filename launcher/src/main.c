#include <stdio.h>
#include <unistd.h>
#include "bzf_http_server.h"

void coucouEndpoint(struct bzf_http_route_output* output);
void rootEndpoint(struct bzf_http_route_output* output);

int main(void) {
    struct bzf_http_server http_server;
    enum bzf_http_server_initialize_result http_server_initialize_result = bzf_http_server_initialize(&http_server);
    if (http_server_initialize_result == BZF_HTTP_SERVER_INITIALIZE_MEMORY_ALLOCATION_ERROR) {
        fprintf(stderr, "memoy allocation error");
        return 42;
    }

    struct bzf_http_route_handler coucou_handler = {{(const bzf_byte_t*)"/coucou", 7}, coucouEndpoint};
    bzf_http_add_route_handler(&http_server, coucou_handler);
    struct bzf_http_route_handler root_handler = {{(const bzf_byte_t*)"/", 1}, rootEndpoint};
    bzf_http_add_route_handler(&http_server, root_handler);
    struct bzf_http_client client;

    bzf_http_server_accept_client(&http_server, &client, NULL);
    bzf_http_handle_request(client, http_server.handlers);
    bzf_http_server_destroy(&http_server);
    fflush(stdout);
    return 0;
}
