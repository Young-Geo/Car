#ifndef __CLIENTSOCKET_H__
#define __CLIENTSOCKET_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include "afx.h"
#include "xchain.h"
#include "xlog.h"
#include "pkt.h"


extern int socket (int domain, int type, int protocol);
extern int bind( int sockfd , const struct sockaddr * my_addr, socklen_t addrlen);
extern int listen( int sockfd, int backlog);
extern in_addr_t inet_addr(const char *cp);
extern char * inet_ntoa(struct in_addr in);
extern int inet_aton(const char *string, struct in_addr*addr);
extern uint32_t htonl(uint32_t hostlong);
extern uint16_t htons(uint16_t hostshort);

extern int select(int nfds, fd_set *readfds, fd_set *writefds, \
						fd_set *exceptfds, struct timeval *timeout);
extern int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
extern ssize_t read(int fd, void *buf, size_t count);
extern ssize_t write(int fd, const void *buf, size_t count);



#define MAXLISTEN 128

enum CONN_FLAG {
	TCP,
	UDP
};

typedef int		(*client_call_back_t)(int fd, unsigned char *buf, int buf_size, void *arg);

typedef struct _client_socket_t {
	int lfd, cfd;
	u8 is_listen;
	xchain *rchain, *wchain;
	///
	client_call_back_t call_back;
	void *arg;
} client_socket_t;


client_socket_t *	client_socket_init(char *ip, int port);

int					client_socket_set_call_back(client_socket_t *csocket, client_call_back_t call_back, void *arg);

int					client_socket_accept(client_socket_t *csocket);

int					client_socket_spinOnce_read(client_socket_t *csocket);

int					client_socket_spinOnce_write(client_socket_t *csocket);

int					client_socket_spinOnce(client_socket_t *csocket);


int					client_socket_push(client_socket_t *csocket, unsigned char *buf, int buf_len);

int 				client_socket_can_read(int fd, int sec, int usec);

int 				client_socket_can_write(int fd, int sec, int usec);

int					client_socket_destory(client_socket_t *client_socket);

#endif
