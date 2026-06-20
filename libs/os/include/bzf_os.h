#ifndef BZF_OS_H
#define BZF_OS_H
#include <stddef.h>
#include <sys/socket.h>

void* bzf_os_malloc(size_t size);
void bzf_os_free(void* ptr);
int bzf_accept(int fd, struct sockaddr* addr, socklen_t* addr_len);
int bzf_setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen);
int bzf_close(int fd);

#endif