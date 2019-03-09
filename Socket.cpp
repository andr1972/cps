#include "Socket.h"



Socket::Socket()
{
#ifdef _WIN32
#else
    sock_addr_len = sizeof(sock_addr);
#endif
}


Socket::~Socket()
{
}

int Socket::getLastError()
{
#ifdef _WIN32
    return WSAGetLastError();
#else
#endif
}