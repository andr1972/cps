#pragma once
#include "Socket.h"

class ServerSocket : public Socket
{
	WSADATA wsaData;
	int iResult;
	struct addrinfo* result = NULL;
	struct addrinfo hints;
public:
	ServerSocket();
	~ServerSocket();
	void resolve(const char* port);
	void listen();
};

