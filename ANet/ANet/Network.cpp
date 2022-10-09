#include "Network.h"

bool Network::Setup()
{
	WSADATA wsaData;
	return (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0);
}


void Network::CleanUp()
{
	WSACleanup();
}
