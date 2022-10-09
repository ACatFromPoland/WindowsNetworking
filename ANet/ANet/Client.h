#pragma once
#include "Network.h"

class Client : public Network
{
public:
	~Client()
	{
		CleanUp();
	}

	bool Begin(const char* ip, u_short port);

	bool Send(const char* buffer, size_t size);

	bool Recv();

	void CleanUp();

private:
	SOCKET serviceSocket;

	sockaddr_in hostAddress;
};