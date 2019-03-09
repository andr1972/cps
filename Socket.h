#pragma once
#define WIN32_LEAN_AND_MEAN

#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

class Socket
{
protected:
	friend class ClientSocket;
#ifdef _WIN32
	SOCKET connectSocket = INVALID_SOCKET;
#else
    int connectSocket = -1;
    struct sockaddr_in sock_addr;
    socklen_t sock_addr_len;
#endif
public:
	Socket();
	~Socket();
	int getLastError();
};

