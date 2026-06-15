#ifndef BZF_OS_H
#define BZF_OS_H
#include <stddef.h>
#include <sys/socket.h>

void* bzf_os_malloc(size_t size);
void bzf_os_free(void* ptr);
int bzf_accept(int fd, struct sockaddr* addr, socklen_t* addr_len);

#endif