#include "Server.h"

bool Server::Begin(u_short port)
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

bool Server::StartupRecv()
{
	recvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (recvSocket == INVALID_SOCKET)
	{
		printf("CLIENT Error creating listener socket ");
		return false;
	}

	return (bind(recvSocket, (sockaddr*)&hostAddress, sizeof(hostAddress)) != SOCKET_ERROR);
}

bool Server::StartupSend()
{
	sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	return (sendSocket != INVALID_SOCKET);
}

void Server::CleanUp()
{
	closesocket(sendSocket);
	closesocket(recvSocket);
	WSACleanup();
}

Server::~Server()
{
	CleanUp();
}

bool Server::SendTo(unsigned char* buffer, size_t size, sockaddr_in& address)
{
	return (sendto(sendSocket, (char*)buffer, (int)size, 0, (SOCKADDR*)&address, (int)sizeof(address)) != SOCKET_ERROR);
}

bool Server::RecvFrom(unsigned char* buffer, size_t size, sockaddr_in& address)
{
	int _addressSize = (int)sizeof(address);
	int iResult = recvfrom(recvSocket, (char*)buffer, (int)size, 0, (SOCKADDR*)&address, &_addressSize);

	return (iResult != SOCKET_ERROR);
}