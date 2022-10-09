#include "ANet/Host.h"

#define RETERR(x) return WSAError(x)
int WSAError(const char* Str)
{
	printf("%s", Str);
	
	char buf[256];
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)buf, sizeof(buf), NULL);
	
	printf(" %ld: %s\n", WSAGetLastError(), buf);

	return 1;
}

int main()
{
	Host Net;
	Net.Setup();

	if (!Net.Begin(5006))
		RETERR("Error starting Server!");



	while (true)
	{
		sockaddr_in fromAddress;

		if (!Net.RecvFrom(fromAddress))
			RETERR("Error at recvfrom");

		// Send Data
		const char* sendbuf = "this is a test";

		if (!Net.SendTo(sendbuf, 15, fromAddress))
			RETERR("SERVER Error at sendto!");
	}

	return 0;
}