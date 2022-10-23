#include "sPacketHandler.h"
#include "sThreading.h"
#include "sWorld.h"

void setConsoleMode();

struct botSpawner
{
	bool shouldSpawn = true;
	Player* bot = nullptr;
	Vector2 position = Vector2(400.f, 100.f);

	void spawnIfDead(World& world)
	{
		if (!bot)
		{
			bot = world.entityHandler.createEntity<Player>();
			bot->position = position;
			bot->classType = 0;
			bot->isBot = true;
			bot->type = ENT_BOT;
		}
		else
		{
			if (bot->toDelete)
				bot = nullptr;
		}
	}
};

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

	NetClock debug = NetClock(1.f);

	botSpawner spawner;
	spawner.spawnIfDead(world);
	printf("[ENT COUNT] %d\n", (int)world.entityHandler.entities.count);

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

		world.checkConnections();
		
		world.update();

		world.updateClients();

		if (debug.Tick())
			printf("[ENT COUNT] %d\n", (int)world.entityHandler.entities.count);
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