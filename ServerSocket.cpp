#include "ServerSocket.h"
#include "Exception.h"
#ifdef _WIN32
#include <ws2tcpip.h>
#endif

const int backLogSize = 128;

ServerSocket::ServerSocket()
{
#ifdef _WIN32
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		throw Exception("WSAStartup failed",iResult);
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
#endif
}

void ServerSocket::resolve(int  port)
{
#ifdef _WIN32
	iResult = getaddrinfo(NULL, to_string(port).c_str(), &hints, &result);
	if (iResult != 0) {
		WSACleanup();
		throw Exception("getaddrinfo failed with error: %d\n",iResult);
	}
#else
	sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    sock_addr.sin_port = htons(port);
#endif
}

void ServerSocket::listen()
{
#ifdef _WIN32
	connectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (connectSocket == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
		throw Exception("socket failed with error: %ld\n", WSAGetLastError());
	}

	// Setup the TCP listening socket
	iResult = bind(connectSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		freeaddrinfo(result);
		closesocket(connectSocket);
		WSACleanup();
		throw Exception("bind failed with error: %d\n", WSAGetLastError());
	}

	freeaddrinfo(result);

	iResult = ::listen(connectSocket, backLogSize);
	if (iResult == SOCKET_ERROR) {
		closesocket(connectSocket);
		WSACleanup();
		throw Exception("listen failed with error: %d\n", WSAGetLastError());
	}
#else
	connectSocket = socket(AF_INET , SOCK_STREAM , 0);
	if (connectSocket == -1)
		throw Exception("Could not create socket");
	if (bind(connectSocket, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
		throw Exception("ERROR on binding");
	::listen(connectSocket,backLogSize);
#endif
}

ServerSocket::~ServerSocket()
{
}
