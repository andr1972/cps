#pragma once
#include "Socket.h"

class ClientSocket: public Socket
{
#ifdef _WIN32
	WSADATA wsaData;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
#endif
public:
	ClientSocket();
	ClientSocket(Socket* sock);
	~ClientSocket();
	void resolve(const char* address, int port);
	void connect();
	int send(const char* sendbuf, int buflen);
	void closeSend();
	int receive(char* receivebuf, int buflen);
};

