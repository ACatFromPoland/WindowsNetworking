#include "Client.h"

Client::~Client()
{
	CleanUp();
}

bool Client::Begin(const char* ip, u_short port)
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

	// TODO: Start Recv Thread

	return true;
}

void Client::CleanUp()
{
	closesocket(serviceSocket);
	WSACleanup();
}

bool Client::Send(unsigned char* buffer, size_t size)
{
	return (sendto(serviceSocket, (char*)buffer, (int)size, 0, (SOCKADDR*)&hostAddress, (int)sizeof(hostAddress)) != SOCKET_ERROR);
}

bool Client::Recv(unsigned char* buffer, size_t size)
{
	int iResult = recv(serviceSocket, (char*)buffer, (int)size, 0);

	return (iResult != SOCKET_ERROR);
}