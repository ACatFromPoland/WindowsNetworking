#include "ANet/Host.h"

int main()
{
	Host Net;
	Net.Setup();

	if (!Net.Begin(5006))
		Net.WSAError("Error starting Server!");

	while (true)
	{
		sockaddr_in fromAddress;

		unsigned char buffer[DEFAULT_BUFLEN];

		if (!Net.RecvFrom(buffer, DEFAULT_BUFLEN, fromAddress))
			return Net.WSAError("Error at recvfrom");

		// Send Data
		const char* sendbuf = "this is a test";

		if (!Net.SendTo(sendbuf, 15, fromAddress))
			return Net.WSAError("SERVER Error at sendto!");
	}

	return 0;
}