#pragma once
#include "Socket.h"

class ClientSocket: public Socket
{
	WSADATA wsaData;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
public:
	ClientSocket();
	ClientSocket(Socket* sock);
	~ClientSocket();
	void resolve(const char* address, const char* port);
	void connect();
	int send(const char* sendbuf, int buflen);
	void closeSend();
	int receive(char* receivebuf, int buflen);
};

