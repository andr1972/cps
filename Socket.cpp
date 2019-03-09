#include "Socket.h"



Socket::Socket()
{
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