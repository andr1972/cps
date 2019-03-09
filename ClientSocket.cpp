#include "ClientSocket.h"
#include "Exception.h"
#ifdef _WIN32
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#else
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif


ClientSocket::ClientSocket()
{
#ifdef _WIN32
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		throw Exception("WSAStartup failed", iResult);
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
#else
	bzero((char *) &sock_addr, sizeof(sock_addr));
#endif
}

ClientSocket::ClientSocket(Socket* sock)
{
#ifdef _WIN32
	// Accept a client socket
	connectSocket = accept(sock->connectSocket, NULL, NULL);
	if (connectSocket == INVALID_SOCKET) {
		closesocket(sock->connectSocket);
		WSACleanup();
		throw Exception("accept failed", WSAGetLastError());
	}
	// No longer need server socket
	closesocket(sock->connectSocket);
#else
	connectSocket = ::accept(sock->connectSocket, (struct sockaddr *) &sock_addr, &sock_addr_len);
#endif
}

void ClientSocket::resolve(const char *address, int port)
{
#ifdef _WIN32
	int iResult = getaddrinfo(address, to_string(port).c_str(), &hints, &result);
	if (iResult != 0)
	{
		WSACleanup();
		throw Exception("getaddrinfo failed", iResult);
	}
#else
	if (inet_pton(AF_INET, address, &(sock_addr.sin_addr))==0)
	{
		struct hostent *host;
		host = gethostbyname(address);
		sock_addr.sin_family = AF_INET;
		bcopy((char *)host->h_addr,
			  (char *)&sock_addr.sin_addr.s_addr,
			  host->h_length);
		sock_addr.sin_port = htons(port);
	}
	else
	{
		sock_addr.sin_addr.s_addr = inet_addr(address);
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_port = htons(port);
	}
#endif
}

void ClientSocket::connect()
{
#ifdef _WIN32
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
#else
	connectSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (::connect(connectSocket,(struct sockaddr *)&sock_addr,sizeof(sock_addr)) < 0)
		throw Exception("ERROR connecting");
#endif
}

int ClientSocket::send(const char* sendbuf, int buflen)
{
#ifdef _WIN32
	int iResult = ::send(connectSocket, sendbuf, buflen, 0);
	if (iResult == SOCKET_ERROR) {
		closesocket(connectSocket);
		WSACleanup();
		throw "send failed with error: %d\n";//WSAGetLastError());
	}
	return iResult;
#else
	return write(connectSocket,sendbuf, buflen);
#endif
}

void ClientSocket::closeSend()
{
#ifdef _WIN32
	// shutdown the connection since no more data will be sent
	int iResult = shutdown(connectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		closesocket(connectSocket);
		WSACleanup();
		throw "shutdown failed with error: %d\n";// , WSAGetLastError());
	}
#else
	shutdown(connectSocket, SHUT_WR);
#endif
}

int ClientSocket::receive(char* receivebuf, int buflen)
{
#ifdef _WIN32
	return recv(connectSocket, receivebuf, buflen, 0);
#else
	return read(connectSocket, receivebuf, buflen);
#endif
}

ClientSocket::~ClientSocket()
{
#ifdef _WIN32
	closesocket(connectSocket);
	WSACleanup();
#else
	close(connectSocket);
#endif
}
