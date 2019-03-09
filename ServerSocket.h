#pragma once
#include "Socket.h"

class ServerSocket : public Socket
{
#ifdef _WIN32
	WSADATA wsaData;
	int iResult;
	struct addrinfo* result = NULL;
	struct addrinfo hints;
#endif
public:
	ServerSocket();
	~ServerSocket();
	void resolve(const char* port);
	void listen();
};

