#pragma once
#define WIN32_LEAN_AND_MEAN

#ifdef _WIN32
#include <winsock2.h>
#endif

class Socket
{
protected:
	friend class ClientSocket;
#ifdef _WIN32
	SOCKET connectSocket = INVALID_SOCKET;
#else
    int connectSocket = -1;
#endif
public:
	Socket();
	~Socket();
	int getLastError();
};

