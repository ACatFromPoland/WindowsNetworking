#include "ANet/Network.h"

bool Network::Setup()
{
	WSADATA wsaData;
	return (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0);
}

void Network::CleanUp()
{
	WSACleanup();
}

int Network::WSAError(const char* Str)
{
	printf("%s", Str);

	char buf[256];
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)buf, sizeof(buf), NULL);

	printf(" %ld: %s\n", WSAGetLastError(), buf);

	return 1;
}