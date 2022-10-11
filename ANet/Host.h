#pragma once
#include "Network.h"

class Host : public Network
{
public:
	~Host();

	bool Begin(u_short port = 0);

	bool SendTo(unsigned char* buffer, size_t size, sockaddr_in& address);

	bool RecvFrom(unsigned char*, size_t size, sockaddr_in& address);

	virtual void CleanUp();

private:

	bool StartupRecv();

	bool StartupSend();

	SOCKET sendSocket;
	SOCKET recvSocket;

	sockaddr_in hostAddress;
	sockaddr_in clientAddress;
};