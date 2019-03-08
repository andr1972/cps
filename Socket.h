#pragma once
#define WIN32_LEAN_AND_MEAN

#ifdef _WIN32
#include <winsock2.h>
#endif

class Socket
{
protected:
	friend class ClientSocket;
	SOCKET connectSocket = INVALID_SOCKET;
public:
	Socket();
	~Socket();
};

