#include "node_client/client_socket.h"




client_socket_t *	client_socket_init(char *ip, int port)
{
	client_socket_t *csocket = NULL;
	struct sockaddr_in server_addr;
	int flag = 0;

	xassert(ip);	
	xassert((csocket = (client_socket_t *)xmalloc(sizeof(client_socket_t))));
	xassert((csocket->rchain = xchain_alloc()));
	xassert((csocket->wchain = xchain_alloc()));

	csocket->lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (csocket->lfd < 0) {
		xerror("SOCKET create error\n");
		goto err;
	}

	flag = fcntl(csocket->lfd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(csocket->lfd, F_SETFL, flag);

	xmemset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons((short)port);

	if (bind(csocket->lfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
		xerror("bind error\n");
		goto err;
	}
	if (listen(csocket->lfd, MAXLISTEN)) {
		xerror("listen error\n");
		goto err;
	}

	csocket->is_listen = 1;
	csocket->cfd = -1;
	csocket->call_back = NULL;
	csocket->arg = NULL;

	return csocket;
err:
	xfree(csocket);
	return NULL;
}

int		client_socket_set_call_back(client_socket_t *csocket, client_call_back_t call_back, void *arg)
{
	XXNULL(csocket, -1);
	XXNULL(call_back, -1);

	if (csocket->call_back) {
		xerror("call_back not seting\n");
		return -1;
	}

	csocket->call_back = call_back;
	csocket->arg = arg;
	return 0;
}

int		client_socket_accept(client_socket_t *csocket)
{
	int flag = 0;
	
	XXNULL(csocket, -1);

	if (csocket->cfd > 0) {
		xmessage("cfd %d not Repeated setting\n", csocket->cfd);
		return -1;
	}

	if (!csocket->is_listen) {
		xerror("fd %d not listen\n", csocket->lfd);
		return -1;
	}

	if (client_socket_can_read(csocket->lfd, 0, 0)) {
		xmessage("fd %d not read\n", csocket->lfd);
		return -1;
	}

	csocket->cfd = accept(csocket->lfd, NULL, NULL);
	if (csocket->cfd < 0) {
		xerror("fd error accept()\n");
		return -1;
	}

	flag = fcntl(csocket->cfd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(csocket->cfd, F_SETFL, flag);
	return 0;
}

int	client_socket_push(client_socket_t *csocket, unsigned char *buf, int buf_len)
{
	#define SOCKET_BUF_MAX Kbyte(100)
	int data_len = 0;

	XXNULL(csocket, -1);
	XXNULL(csocket->wchain, -1);

	if (!buf || buf_len < 0) {
		xerror("buf error\n");
		return -1;
	}

	if ((data_len = (xchain_size(csocket->wchain) + buf_len)) > SOCKET_BUF_MAX) {
		xmessage("chain too big clear\n");
		xchain_delete(csocket->wchain, xchain_size(csocket->wchain));
	}
	xchain_add(csocket->wchain, buf, buf_len);

	return 0;
}


int	client_socket_spinOnce_read(client_socket_t *csocket)
{	
	unsigned char data[Mbyte(1)] = { 0 };
	int _size = 0;
	
	XXNULL(csocket, -1);
	XXNULL(csocket->rchain, -1);

	if (csocket->cfd < 0 && client_socket_accept(csocket)) {
		xerror("read spin: cfd init error\n");
		return -1;
	}

	if (client_socket_can_read(csocket->cfd, 0, 0)) {
		xmessage("read spin: cfd %d no can read\n", csocket->cfd);
		return -2;
	}

	while (1)
	{
		_size = read(csocket->cfd, data, sizeof(data));
		if (_size == -1 && EBADF == errno) {
			xmessage("read spin: read close fd %d\n", csocket->cfd);
			close(csocket->cfd);
			csocket->cfd = -1;
		}
		
		if (_size <= 0)
			break;
		
		xchain_add(csocket->rchain, data, _size);
	}

	if (!csocket->call_back) {
		xerror("read spin: call_back error\n");
		return -1;
	}

	if ((_size = xchain_size(csocket->rchain)) <= 0) {
		xerror("read spin: chain not data\n");
		return -1;
	}
	xchain_get(csocket->rchain, data, _size);
	xchain_delete(csocket->rchain, _size);
	
	xmessage("read spin: read %d chain size %d\n", _size, xchain_size(csocket->rchain));
	
	csocket->call_back(csocket->cfd, data, _size, csocket->arg);
	
	return 0;
}

int					client_socket_spinOnce_write(client_socket_t *csocket)
{
	unsigned char data[Kbyte(100)] = { 0 }, out_data[Kbyte(100)] = { 0 };
	int data_len = 0, ret = 0, wcount = 0;
	XXNULL(csocket, -1);
	XXNULL(csocket->wchain, -1);


	if (csocket->cfd < 0 && client_socket_accept(csocket)) {
		xerror("write spin: cfd init error\n");
		return -1;
	}

	if (client_socket_can_write(csocket->cfd, 0, 0)) {
		xmessage("write spin: fd %d not write\n", csocket->cfd);
		return -2;
	}

	if ((data_len = xchain_size(csocket->wchain)) <= 0) {
		xmessage("write spin: chain not data\n");
		return -1;
	}

	if (data_len > Kbyte(100)) {
		xmessage("write spin: chain data too big\n");
		xchain_delete(csocket->wchain, data_len);
		return -1;
	}
	
	xchain_get(csocket->wchain, data, data_len);
	xchain_delete(csocket->wchain, data_len);

	//data_len = pkt_make_frame(data, data_len, out_data, sizeof(out_data));
	//if (data_len <= 0) {
		//xerror("pkt_make_frame %d\n", data_len);
		//return -1;
	//}


	while (1)
	{
		ret = write(csocket->cfd, data + wcount, data_len - wcount);
		xmessage("write spin: %d data_size %d errno %d\n", csocket->cfd, ret, errno);
		if (-1 == ret && (EBADF == errno)) {
			xmessage("write spin: close fd %d\n", csocket->cfd);
			close(csocket->cfd);
			csocket->cfd = -1;
			break;
		}		
		wcount += ret;
		if (wcount >= data_len)
			break;
	}

	return 0;
}


int					client_socket_spinOnce(client_socket_t *csocket)
{
	int ret = 0;
	
	XXNULL(csocket, -1);
	XXNULL(csocket->rchain, -1);
	XXNULL(csocket->wchain, -1);
	XXNULL(csocket->call_back, -1);

	
	ret = client_socket_spinOnce_read(csocket);
	if (!ret) {
		xmessage("client_socket_spinOnce_read ok %d\n", ret);
	}

	ret = client_socket_spinOnce_write(csocket);
	if (!ret) {
		xmessage("client_socket_spinOnce_write ok %d\n", ret);
	}
	return 0;
}

int client_socket_can_read(int fd, int sec, int usec)
{
	  int maxfd = 0, ret = 0;
	  fd_set rset;
	  struct timeval tv;
	  
	  if (fd < 0) return 0 ;
	  
	  tv.tv_sec = sec ;
	  tv.tv_usec = usec ;
	  
	  FD_ZERO(&rset);
	  FD_SET(fd, &rset);
	  
	  maxfd = fd + 1;
	  
	__select_call:
	  ret = select (maxfd, &rset, NULL, NULL, &tv) ;
	  if (ret < 0)
	  {
	    if ( errno == EINTR ) goto __select_call ;
	    return -1;
	  }
	  if (ret == 0) return -1;
	  if (FD_ISSET(fd, &rset)) return 0;
	  else return -1;
}


int client_socket_can_write(int fd, int sec, int usec)
{
	  int maxfd = 0, ret = 0;
	  fd_set wset;
	  struct timeval tv;
	  
	  if (fd < 0) return 0 ;
	  
	  tv.tv_sec = sec ;
	  tv.tv_usec = usec ;
	  
	  FD_ZERO(&wset);
	  FD_SET(fd, &wset);
	  
	  maxfd = fd + 1;
	  
	__select_call:
	  ret = select (maxfd, NULL, &wset, NULL, &tv) ;
	  if (ret < 0)
	  {
	    if ( errno == EINTR ) goto __select_call ;
	    return -1;
	  }
	  if (ret == 0) return -1;
	  if (FD_ISSET(fd, &wset)) return 0;
	  else return -1;
}


int					client_socket_destory(client_socket_t *client_socket)
{
	xassert(client_socket);
	xassert(client_socket->rchain);
	xassert(client_socket->wchain);
	xchain_clear(client_socket->rchain);
	xchain_clear(client_socket->wchain);
	client_socket->is_listen = 0;
	if (client_socket->lfd > 0) close(client_socket->lfd);
	if (client_socket->cfd > 0) close(client_socket->cfd);
	xfree(client_socket);
}


