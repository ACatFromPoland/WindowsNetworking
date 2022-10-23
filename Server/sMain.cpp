#include "sPacketHandler.h"
#include "sThreading.h"
#include "sWorld.h"

void setConsoleMode();

int main()
{
	setConsoleMode();

	// Opens a server on port 5006
	NetServer net;
	net.Setup();
	net.Begin(5006);

	// Opens a listener thread for the server
	DynamicArray<packetData> packetQueue;
	std::thread recvThread(packetThread, &net, &packetQueue);

	// Starts the game
	World world = World(&net);

	while (true)
	{
		// Handle new packets
		net.threadLock.lock();
		for (packetData& packet : packetQueue)
		{
			handlePacket(packet, world);
		}
		packetQueue.purge();
		net.threadLock.unlock();

		// Check player connection
		world.checkConnections();
		
		world.update();

		world.updateClients();
	}

	return 0;
}

void setConsoleMode()
{
	HANDLE conHandle = GetStdHandle(-10);
	DWORD mode;
	if (!GetConsoleMode(conHandle, &mode))
		printf("No Console?\n");
	mode = mode & ~(64 | 128); // Disable QuickEditMode
	if (!SetConsoleMode(conHandle, mode))
		printf("Can't Set Console Mode!\n");
}