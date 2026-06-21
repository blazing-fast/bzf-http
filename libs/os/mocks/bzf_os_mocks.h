#ifndef BZF_OS_MOCKS_H
#define BZF_OS_MOCKS_H

#include <stddef.h>
#include <sys/socket.h>
#include <sys/types.h>

void mock_bzf_os_malloc_set(void* (*fn)(size_t));
void mock_bzf_os_free_set(void (*fn)(void*));

void mock_bzf_socket_set(int (*fn)(int, int, int));
void mock_bzf_bind_set(int (*fn)(int, const struct sockaddr*, socklen_t));
void mock_bzf_listen_set(int (*fn)(int, int));
void mock_bzf_accept_set(int (*fn)(int, struct sockaddr*, socklen_t*));
void mock_bzf_send_set(ssize_t (*fn)(int, const void*, size_t, int));
void mock_bzf_setsockopt_set(int (*fn)(int, int, int, const void*, socklen_t));
void mock_bzf_close_set(int (*fn)(int));

#endif
