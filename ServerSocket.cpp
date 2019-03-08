#include "ServerSocket.h"
#include "Exception.h"
#ifdef _WIN32
#include <ws2tcpip.h>
#endif

ServerSocket::ServerSocket()
{
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		throw Exception("WSAStartup failed",iResult);
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
}

void ServerSocket::resolve(const char* port)
{
	// Resolve the server address and port
	iResult = getaddrinfo(NULL, port, &hints, &result);
	if (iResult != 0) {		
		WSACleanup();
		throw Exception("getaddrinfo failed with error: %d\n",iResult);
	}
}

void ServerSocket::listen()
{
	// Create a SOCKET for connecting to server
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

	iResult = ::listen(connectSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {		
		closesocket(connectSocket);
		WSACleanup();
		throw Exception("listen failed with error: %d\n", WSAGetLastError());
	}
}

ServerSocket::~ServerSocket()
{
}
