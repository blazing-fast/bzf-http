#ifndef BZF_HTTP_SERVER_H
#define BZF_HTTP_SERVER_H
#include "bzf_bytes.h"
#include "bzf_dynamic_array.h"
#include "bzf_hashmap.h"
#include <netinet/in.h>

struct bzf_http_route_output {
    struct bzf_bytes_mutable_buffer* response;
};

enum bzf_http_method {
    BZF_HTTP_METHOD_GET, BZF_HTTP_METHOD_POST, BZF_HTTP_METHOD_PUT, BZF_HTTP_METHOD_PATCH, BZF_HTTP_METHOD_DELETE
};

struct bzf_http_route_handler {
    struct bzf_bytes_immutable_view route;
    void (*handler)(struct bzf_http_route_output*);
};

struct bzf_http_handlers
{
    struct bzf_hashmap* configured_routes;
    void (*default_handler)(struct bzf_http_route_output*);
};

/* Opaque forward declaration. Real definition lives in src/bzf_http_server_impl.h. */
struct bzf_http_server;

struct bzf_http_client {
    int file_descriptor;
};

enum bzf_http_server_initialize_result {
    BZF_HTTP_SERVER_INITIALIZE_OK,
    BZF_HTTP_SERVER_INITIALIZE_MEMORY_ALLOCATION_ERROR,
    BZF_HTTP_SERVER_INITIALIZE_SOCKET_INITIALISATION_ERROR,
};
enum bzf_http_server_initialize_result bzf_http_server_initialize(struct bzf_http_server** out);

enum bzf_http_add_route_handler_result {
    BZF_HTTP_ADD_ROUTE_HANDLER_OK,
    BZF_HTTP_ADD_ROUTE_HANDLER_MEMORY_ALLOCATION_ERROR,
};
enum bzf_http_add_route_handler_result bzf_http_add_route_handler(const struct bzf_http_server* http_server, struct bzf_http_route_handler route_handler);

enum bzf_http_server_accept_client_result {
    BZF_HTTP_SERVER_ACCEPT_CLIENT_OK,
    BZF_HTTP_SERVER_ACCEPT_ERROR_WHEN_ACCEPTING,
    BZF_HTTP_SERVER_ACCEPT_CLIENT_ERROR_WHEN_SETSOCKETOPT
};
enum bzf_http_server_accept_client_result bzf_http_server_accept_client(struct bzf_http_server* http_server, struct bzf_http_client* client, struct sockaddr_in* client_addr);

void bzf_http_server_destroy(struct bzf_http_server* http_server);

enum bzf_http_send_response_result{BZF_HTTP_SEND_RESPONSE_OK, BZF_HTTP_SEND_RESPONSE_CLOSED, BZF_HTTP_SEND_RESPONSE_ERROR};
enum bzf_http_send_response_result bzf_http_send_response(const struct bzf_http_client bzf_http_client, const struct bzf_bytes_immutable_view to_send);

enum bzf_http_server_accept_and_handle_result {
    BZF_HTTP_SERVER_ACCEPT_AND_HANDLE_OK,
    BZF_HTTP_SERVER_ACCEPT_AND_HANDLE_ACCEPT_ERROR,
    BZF_HTTP_SERVER_ACCEPT_AND_HANDLE_REQUEST_ERROR,
};
enum bzf_http_server_accept_and_handle_result bzf_http_server_accept_and_handle(struct bzf_http_server* server);

void bzf_http_server_run(struct bzf_http_server* server);

#endif /* BZF_HTTP_SERVER_H */
