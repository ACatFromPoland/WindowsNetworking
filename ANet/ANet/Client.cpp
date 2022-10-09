#include "Client.h"

bool Client::Begin(const char* ip, u_short port)
{
	// Host Address Setup
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
}

void Client::CleanUp()
{
	closesocket(serviceSocket);
	WSACleanup();
}

bool Client::Send(const char* buffer, size_t size)
{
	return (sendto(serviceSocket, buffer, size, 0, (SOCKADDR*)&hostAddress, sizeof(hostAddress)) != SOCKET_ERROR);
}

bool Client::Recv()
{
	char recvbuf[DEFAULT_BUFLEN];
	memset(recvbuf, 0, DEFAULT_BUFLEN);

	int iResult = recv(serviceSocket, recvbuf, DEFAULT_BUFLEN, 0);

	printf("%s\n", recvbuf);

	return (iResult != SOCKET_ERROR);
}

