#include "ClientSocket.h"
#include "Exception.h"
#ifdef _WIN32
#include <ws2tcpip.h>
#endif

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


ClientSocket::ClientSocket()
{
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		throw Exception("WSAStartup failed", iResult);
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
}

ClientSocket::ClientSocket(Socket* sock)
{
	// Accept a client socket
	connectSocket = accept(sock->connectSocket, NULL, NULL);
	if (connectSocket == INVALID_SOCKET) {		
		closesocket(sock->connectSocket);
		WSACleanup();
		throw Exception("accept failed", WSAGetLastError());
	}
	// No longer need server socket
	closesocket(sock->connectSocket);
}

void ClientSocket::resolve(const char *address, const char *port)
{
	int iResult = getaddrinfo(address, port, &hints, &result);
	if (iResult != 0) 
	{
		WSACleanup();
		throw Exception("getaddrinfo failed", iResult);
	}
}

void ClientSocket::connect()
{
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		// Create a SOCKET for connecting to server
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (connectSocket == INVALID_SOCKET) {			
			WSACleanup();
			throw Exception("socket failed with error: %ld\n", WSAGetLastError());
		}
		// Connect to server.
		int iResult = ::connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);
	if (connectSocket == INVALID_SOCKET) {
		WSACleanup();
		throw Exception("Unable to connect to server!\n");
	}
}

int ClientSocket::send(const char* sendbuf, int buflen)
{
	// Send an initial buffer
	int iResult = ::send(connectSocket, sendbuf, buflen, 0);
	if (iResult == SOCKET_ERROR) {		
		closesocket(connectSocket);
		WSACleanup();
		throw "send failed with error: %d\n";//WSAGetLastError());
	}
	return iResult;
}

void ClientSocket::closeSend()
{
	// shutdown the connection since no more data will be sent
	int iResult = shutdown(connectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		closesocket(connectSocket);
		WSACleanup();
		throw "shutdown failed with error: %d\n";// , WSAGetLastError());
	}
}

int ClientSocket::receive(char* receivebuf, int buflen)
{
	return recv(connectSocket, receivebuf, buflen, 0);
}

ClientSocket::~ClientSocket()
{
	// cleanup
	closesocket(connectSocket);
	WSACleanup();
}
