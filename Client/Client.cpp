#include "ANet/Client.h"

int main()
{
	Client Net;
	Net.Setup();

	Net.Begin("127.0.0.1", 5006);

	unsigned char sendBuffer[DEFAULT_BUFLEN];
	
	if (!Net.Send(sendBuffer, DEFAULT_BUFLEN))
		return Net.WSAError("Couldn't send data!");

	unsigned char recvBuffer[DEFAULT_BUFLEN];

	if (!Net.Recv(recvBuffer, DEFAULT_BUFLEN))
		return Net.WSAError("CLIENT Error at recv!");
	
	system("pause");
	return 0;
}