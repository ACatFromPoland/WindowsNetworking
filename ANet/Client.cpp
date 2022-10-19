#include "Client.h"

NetClient::~NetClient()
{
	CleanUp();
}



bool NetClient::Begin(const char* ip, u_short port)
{
	// Server Address Setup
	hostAddress.sin_family = AF_INET;

	inet_pton(AF_INET, ip, &hostAddress.sin_addr);
	hostAddress.sin_port = htons(port);

	// Open Socket

	serviceSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serviceSocket == INVALID_SOCKET)
	{
		printf("Error creating socket ");
		return false;
	}

	return true;
}

bool NetClient::Send(unsigned char* buffer, size_t size)
{
	return (sendto(serviceSocket, (char*)buffer, (int)size, 0, (SOCKADDR*)&hostAddress, (int)sizeof(hostAddress)) != SOCKET_ERROR);
}

bool NetClient::Recv(unsigned char* buffer, size_t size)
{
	int iResult = recv(serviceSocket, (char*)buffer, (int)size, 0);

	return (iResult > 0);
}

void NetClient::CleanUp()
{
	closesocket(serviceSocket);
	WSACleanup();
}