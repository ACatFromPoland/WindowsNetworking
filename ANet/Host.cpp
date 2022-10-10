#include "ANet/Host.h"

bool Host::Begin(u_short port)
{
	// Setup Listener Socket
	hostAddress.sin_addr.S_un.S_addr = ADDR_ANY;
	hostAddress.sin_family = AF_INET;

	if (port)
		hostAddress.sin_port = htons(port);

	if (!StartupRecv())
		return false;

	// Setup Sender Socket
	if (!StartupSend())
		return false;

	// TODO: Start Thread
	return true;
}

bool Host::StartupRecv()
{
	recvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (recvSocket == INVALID_SOCKET)
	{
		printf("CLIENT Error creating listener socket ");
		return false;
	}

	return (bind(recvSocket, (sockaddr*)&hostAddress, sizeof(hostAddress)) != SOCKET_ERROR);
}

bool Host::StartupSend()
{
	sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	return (sendSocket != INVALID_SOCKET);
}

void Host::CleanUp()
{
	closesocket(sendSocket);
	closesocket(recvSocket);
	WSACleanup();
}

Host::~Host()
{
	CleanUp();
}


bool Host::SendTo(const char* buffer, size_t size, sockaddr_in& address)
{
	return (sendto(sendSocket, buffer, (int)size, 0, (SOCKADDR*)&address, (int)sizeof(address)) != SOCKET_ERROR);
}

bool Host::RecvFrom(unsigned char*, size_t size, sockaddr_in& address)
{
	char recvbuf[DEFAULT_BUFLEN];
	memset(recvbuf, 0, DEFAULT_BUFLEN);

	int _addressSize = sizeof(address);
	int iResult = recvfrom(recvSocket, recvbuf, DEFAULT_BUFLEN, 0, (SOCKADDR*)&address, &_addressSize);

	printf("%s\n", recvbuf);

	return (iResult != SOCKET_ERROR);
}