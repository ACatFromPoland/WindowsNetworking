#include "ANet/Host.h"
#include "ANet/NetVar.h"
#include "ANet/NetBuffer.h"

#include "Shared/Clock.h"

#include "ServerNet.h"
#include "Enviroment.h"

int main()
{
	Host Net;
	Net.Setup();

	NetClock clock(66.0f);
	Enviroment World;

	if (!Net.Begin(5006))
		Net.WSAError("Error starting Server!");

	std::thread netThread(serverThread, Net);

	while (true)
	{
		if (netThread.joinable())
			return 1;

		// Update
		World.Update();

		Net.ThreadLock.lock();
		for (PacketData& packet : toRecvPackets)
		{
			HandlePacket(packet, Net, World);
		}
		toRecvPackets.purge();
		Net.ThreadLock.unlock();

		// Send Data
		NetBuffer sendBuffer;

		/*
		if (!Net.SendTo(sendBuffer.buffer, NetBuffer::size, fromAddress))
			return Net.WSAError("SERVER Error at sendto!");
			*/
	}

	return 0;
}