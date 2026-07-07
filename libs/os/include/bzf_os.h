#ifndef BZF_OS_H
#define BZF_OS_H
#include <stddef.h>
#include <sys/socket.h>
#include <sys/types.h>

void* bzf_os_malloc(size_t size);
void* bzf_os_calloc(size_t nmemb, size_t size);
void bzf_os_free(void* ptr);

int bzf_socket(int domain, int type, int protocol);
int bzf_bind(int fd, const struct sockaddr *addr, socklen_t len);
int bzf_listen(int fd, int backlog);
int bzf_accept(int fd, struct sockaddr* addr, socklen_t* addr_len);
ssize_t bzf_send(int fd, const void *buf, size_t n, int flags);
int bzf_setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen);
int bzf_close(int fd);

#endif
