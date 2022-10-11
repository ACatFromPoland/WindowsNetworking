#include "ANet/Client.h"
#include "ANet/NetVar.h"
#include "ANet/NetBuffer.h"

int main()
{
	// Client Send
	Client Net;
	Net.Setup();

	Net.Begin("127.0.0.1", 5006);

	NetBuffer sendBuffer;
	
	if (!Net.Send(sendBuffer.buffer, NetBuffer::size))
		return Net.WSAError("Couldn't send data!");


	// Client Recv
	NetBuffer recvBuffer;

	if (!Net.Recv(recvBuffer.buffer, NetBuffer::size))
		return Net.WSAError("CLIENT Error at recv!");

	system("pause");
	return 0;
}