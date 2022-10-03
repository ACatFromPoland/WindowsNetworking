#pragma once
#include "ANET.h"

int ANetwork::ID = 0;
SOCKET ANetwork::sock = { 0 };
sockaddr_in ANetwork::local = { 0 };
std::mutex ANetwork::threadLock;

void ANetwork::Setup()
{
	ANetwork::ID = 0;
	WSADATA wsad;
	WORD version = MAKEWORD(2, 2);
	if (WSAStartup(version, &wsad))
		printf("[WSA] Could not init Winsock!\n");
}

void ANetwork::FormatAddress(sockaddr_in& address, const char* ip, u_short port)
{
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, ip, &address.sin_addr);
}

void ANetwork::GetIPAddress(sockaddr_in& address, char* ip)
{
	inet_ntop(AF_INET, (void*)&address.sin_addr, ip, 16);
}

void ANetwork::Bind(u_short port)
{
	ANetwork::sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	ANetwork::local.sin_addr.S_un.S_addr = ADDR_ANY;
	ANetwork::local.sin_family = AF_INET;

	if (port)
		ANetwork::local.sin_port = htons(port);

	if (bind(ANetwork::sock, (sockaddr*)&ANetwork::local, sizeof(ANetwork::local)) == SOCKET_ERROR)
		printf("[SOCKET] Could not bind ASockS!\n");
}

int ANetwork::Send(sockaddr_in& address, byte* data, size_t size)
{
	return sendto(ANetwork::sock, (char*)data, (int)size, 0, (sockaddr*)&address, sizeof(address));
}

byte* ANetwork::Recv(sockaddr_in& address, int& bytesRecieved)
{
	byte* tempBuffer = (byte*)malloc(PACKET_MAX_SIZE);

	int addressSize = sizeof(address);
	bytesRecieved = recvfrom(ANetwork::sock, (char*)tempBuffer, PACKET_MAX_SIZE, 0, (SOCKADDR*)&address, &addressSize);
	
	if (bytesRecieved == -1)
	{
		printf("%d\n", bytesRecieved);
		printf("[ERROR] %d\n", WSAGetLastError());
		system("pause");
		return nullptr;
	}

	return tempBuffer;
}

void ANetwork::Close()
{
	WSACleanup();
}
