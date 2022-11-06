#pragma once
#include "NetBase.h"

class NetServer : public NetBase
{
public:
	~NetServer();

	bool Begin(u_short port = 0);

	bool SendTo(unsigned char* buffer, size_t size, sockaddr_in& address);

	bool RecvFrom(unsigned char*, size_t size, sockaddr_in& address);

	virtual void CleanUp();

private:
	// Abstraction
	bool StartupRecv();
	bool StartupSend();


	SOCKET sendSocket;
	SOCKET recvSocket;

	sockaddr_in hostAddress;
	sockaddr_in clientAddress;
};