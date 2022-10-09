#pragma once
#include "Network.h"

class Host : public Network
{
public:
	~Host();

	bool Begin(u_short port = 0);

	bool SendTo(const char* buffer, size_t size, sockaddr_in& address);

	bool RecvFrom(sockaddr_in& address);

	void CleanUp();

private:

	bool StartupRecv();

	bool StartupSend();

	SOCKET sendSocket;
	SOCKET recvSocket;

	sockaddr_in hostAddress;
	sockaddr_in clientAddress;
};