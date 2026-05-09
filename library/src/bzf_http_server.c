#include "bzf_http_server.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>


#include "http.h"
#include "utilities.h"
#include "bzf_bytes.h"

#define SERVER_PORT 12345
#define READ_BUFFER_SIZE 1024
#define MAX_HEADER_SIZE 8192

enum bzf_handle_request_result http_fetch_and_parse_error_to_request_error(
    const enum http_fetch_and_parse_head_result http_fetch_and_parse_head_result);

void printf_bytes_t(const struct bzf_bytes_immutable_view bytes)
{
    printf("%.*s\n", (int)bytes.length, (const char*)bytes.buffer);
}

void defaultHandler(struct bzf_http_route_output* output) {
    static const unsigned char response[] =
    "HTTP/1.1 404 NOT FOUND\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 9\r\n"   // "Not found" = 4 bytes
    "Connection: close\r\n"
    "\r\n"
    "Not found";
    struct bzf_bytes_mutable_buffer mutable_buffer ={(bzf_byte_t*)response, sizeof(response)-1, sizeof(response)-1 };
    *output->response = mutable_buffer;
}

enum bzf_http_server_initialize_result bzf_http_server_initialize(struct bzf_http_server* out)
{
    assert(out != NULL);

    out->file_descriptor = -1;
    const int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        return BZF_HTTP_SERVER_INITIALIZE_SOCKET_INITIALISATION_ERROR;
    }


    const int reuseaddr_opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_opt, sizeof(reuseaddr_opt));

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

    out->file_descriptor = server_fd;
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        fprintf(stderr, "bind failed: %s\n", strerror(errno));
        return BZF_HTTP_SERVER_INITIALIZE_SOCKET_INITIALISATION_ERROR;
    }

    if (listen(server_fd, 1) < 0)
    {
        fprintf(stderr, "listen failed: %s\n", strerror(errno));
        return BZF_HTTP_SERVER_INITIALIZE_SOCKET_INITIALISATION_ERROR;
    }

    out->handlers.default_handler = defaultHandler;
    const enum bzf_hashmap_initialize_result hashmap_initialize_result = bzf_hashmap_initialize(&out->handlers.configured_routes);
    if (hashmap_initialize_result == BZF_HASHMAP_INITIALIZE_ALLOCATION_ERROR)
    {
        return BZF_HTTP_SERVER_INITIALIZE_MEMORY_ALLOCATION_ERROR;
    }
    return BZF_HTTP_SERVER_INITIALIZE_OK;
}

enum bzf_http_server_accept_client_result bzf_http_server_accept_client(struct bzf_http_server* http_server, struct bzf_http_client* client,
                                                                        struct sockaddr_in* client_addr)
{
    assert(http_server != NULL);
    assert(client != NULL);
    int *client_fd = &client->file_descriptor;
    *client_fd = -1;
    socklen_t addr_len = client_addr == NULL ? 0 : sizeof(struct sockaddr_in);
    const int accept_client_fd = accept(http_server->file_descriptor, (struct sockaddr*)client_addr, &addr_len);
    if (accept_client_fd < 0)
    {
        return BZF_HTTP_SERVER_ACCEPT_ERROR_WHEN_ACCEPTING;
    }
    *client_fd = accept_client_fd;

    const struct timeval tv = {5, 0};
    if (setsockopt(*client_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)))
    {
        fprintf(stderr, "setsockopt failed ! You PROBABLY have a logic error in your code ; %s", strerror(errno));
        return BZF_HTTP_SERVER_ACCEPT_CLIENT_ERROR_WHEN_SETSOCKETOPT;
    }
    printf("client fd : %d\n", *client_fd);

    return BZF_HTTP_SERVER_ACCEPT_CLIENT_OK;
}

enum bzf_http_add_route_handler_result bzf_http_add_route_handler(struct bzf_http_server* http_server,
                                                               const struct bzf_http_route_handler route_handler)
{
    assert(http_server != NULL);
    // a malloc per insert is shitty, I know I hate it too !!
    struct bzf_http_route_handler* value = malloc(sizeof(struct bzf_http_route_handler));
    if (value == NULL)
    {
        return BZF_HTTP_ADD_ROUTE_HANDLER_MEMORY_ALLOCATION_ERROR;
    }

    *value = route_handler;
    const enum bzf_hashmap_insert_result hashmap_insert_result = bzf_hashmap_insert(
        http_server->handlers.configured_routes, route_handler.route, (void*)value);
    if (hashmap_insert_result == BZF_HASHMAP_INSERT_ALLOCATION_ERROR)
    {
        return BZF_HTTP_ADD_ROUTE_HANDLER_MEMORY_ALLOCATION_ERROR;
    }
    return BZF_HTTP_ADD_ROUTE_HANDLER_OK;
}

void bzf_http_free_base(struct bzf_bytes_immutable_view key, void* data)
{
    (void)key;
    free(data);
}

enum bzf_http_send_response_result bzf_http_send_response(const struct bzf_http_client bzf_http_client,
                                                          const struct bzf_bytes_immutable_view to_send)
{
    size_t sent_bytes = 0;

    while (sent_bytes < to_send.length)
    {
        ssize_t n = send(bzf_http_client.file_descriptor, to_send.buffer + sent_bytes, to_send.length - sent_bytes,
                         MSG_NOSIGNAL);
        if (n > 0)
        {
            sent_bytes += (size_t)n;
            continue;
        }
        if (n == 0)
        {
            // Shouldn't happen for send(), but treat as closed
            return BZF_HTTP_SEND_RESPONSE_CLOSED;
        }
        switch (errno)
        {
        case EINTR: continue; // retry immediately
        case EPIPE:
        case ECONNRESET:
#ifdef ETIMEDOUT
        case ETIMEDOUT:
#endif
            return BZF_HTTP_SEND_RESPONSE_CLOSED;
        /* Programmer / unexpected errors */
        case EBADF:
        case ENOTSOCK:
        case ENOTCONN:
        case EINVAL:
        case EOPNOTSUPP:
        default:
            return BZF_HTTP_SEND_RESPONSE_ERROR;
        }
    }
    return BZF_HTTP_SEND_RESPONSE_OK;
}


void bzf_http_server_destroy(struct bzf_http_server* http_server)
{
    assert(http_server != NULL);

    if (http_server->file_descriptor >= 0)
    {
        close(http_server->file_descriptor);
    }

    bzf_hashmap_free(http_server->handlers.configured_routes, &bzf_http_free_base);
}



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

enum bzf_handle_request_result bzf_http_handle_request(const struct bzf_http_client client, struct bzf_http_handlers handlers)
{
    enum { HEADERS_INTERNAL_BUFFER_CAPACITY = 20000 };
    bzf_byte_t headers_internal_buffer[HEADERS_INTERNAL_BUFFER_CAPACITY];
    struct bzf_bytes_mutable_buffer headers_raw_buffer = {
        .buffer = headers_internal_buffer, .length = 0, .capacity = HEADERS_INTERNAL_BUFFER_CAPACITY
    };
    struct http_fetch_and_parse_head_output http_fetch_and_parse_head_output;
    const enum bzf_hashmap_initialize_result hashmap_initialize_result = bzf_hashmap_initialize(
        &http_fetch_and_parse_head_output.headers);
    if (hashmap_initialize_result == BZF_HASHMAP_INITIALIZE_ALLOCATION_ERROR)
    {
        return BZF_HANDLE_REQUEST_MEMORY_ALLOCATION_ERROR;
    }


    const enum http_fetch_and_parse_head_result http_fetch_headers_res = http_fetch_and_parse_head(
        client.file_descriptor, &headers_raw_buffer, &http_fetch_and_parse_head_output);

    if (http_fetch_headers_res != HTTP_FETCH_AND_PARSE_HEAD_OK)
    {
        printf("lala %d\n", http_fetch_headers_res);
        return http_fetch_and_parse_error_to_request_error(http_fetch_headers_res);
    }

    struct bzf_bytes_immutable_view host_key_immutable_view = {(const bzf_byte_t*)"host", 4};
    void* tmp;
    const enum bzf_hashmap_get_result hashmap_get_host_result = bzf_hashmap_get(
        http_fetch_and_parse_head_output.headers, host_key_immutable_view, &tmp);
    if (hashmap_get_host_result == BZF_HASHMAP_GET_FOUND)
    {
        printf("found\n");
        const struct bzf_bytes_immutable_view* host_value_view = tmp;
        printf_bytes_t(*host_value_view);
    }

    printf_bytes_t(
        bzf_bytes_immutable_view_from_mutable_buffer_and_range(headers_raw_buffer,
                                                               http_fetch_and_parse_head_output.request_line.target));

    printf("\n");


    const struct bzf_bytes_immutable_view target = bzf_bytes_immutable_view_from_mutable_buffer_and_range(
        headers_raw_buffer, http_fetch_and_parse_head_output.request_line.target);
    const enum bzf_hashmap_get_result hashmap_get_target_result = bzf_hashmap_get(
        handlers.configured_routes, target, &tmp);
    struct bzf_bytes_mutable_buffer mutable_buffer;
    struct bzf_http_route_output route_output = {&mutable_buffer};
    if (hashmap_get_target_result == BZF_HASHMAP_GET_FOUND)
    {
        const struct bzf_http_route_handler* handler = tmp;
        handler->handler(&route_output);
    }
    else
    {
        handlers.default_handler(&route_output);
    }
    bzf_http_send_response(client, bzf_bytes_immutable_view_from_mutable_buffer(*route_output.response));
    bzf_hashmap_free(http_fetch_and_parse_head_output.headers, &bzf_http_free_base);


    close(client.file_descriptor);
    return BZF_HANDLE_REQUEST_OK;
}