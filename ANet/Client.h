#pragma once
#include "Network.h"

class Client : public Network
{
public:
	bool connected;

	~Client();

	bool Begin(const char* ip, u_short port);

	bool Send(unsigned char* buffer, size_t size);

	bool Recv(unsigned char* buffer, size_t size);

	virtual void CleanUp();

private:

	SOCKET serviceSocket;

	sockaddr_in hostAddress;
};