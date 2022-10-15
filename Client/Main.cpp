#include "Client.h" // SFML GRAPHICS NEED THIS AT THE TOP

void establishConnection(Client& net)
{
    if (g_netSendBuffer.isEmpty())
    {
        BUFFER_DATA(g_netSendBuffer, (int)0);
    }

    // Add Connect Packet
    BUFFER_HEADER(g_netSendBuffer, HeaderType::CONNECT);
    {
        unsigned char classId = 0;
        unsigned char weaponId = 0;

//#define RELEASE
#ifdef RELEASE
        std::cout << "Enter Class ID...\n";
        std::cin >> classId;
        std::cout << "Enter Weapon ID...\n";
        std::cin >> weaponId;
#endif

        BUFFER_DATA(g_netSendBuffer, classId);
        BUFFER_DATA(g_netSendBuffer, weaponId);
    }

    net.Send(g_netSendBuffer.buffer, NetBuffer::size);

    net.Recv(g_netRecvBuffer.buffer, NetBuffer::size);
}

int main()
{   
    Client net;
    World world;
    NetClock clock;

    net.Setup();

    if (!net.Begin("127.0.0.1", 5006))
        net.WSAError("Error starting Server!");
    establishConnection(net);

    world.handlePacket(g_netRecvBuffer);
    g_netRecvBuffer.clear();

    // TODO: Start Recv Thread
    sf::RenderWindow window(sf::VideoMode(900, 900), "Mvm");
    std::thread netThread(clientThread, &net);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color(146, 146, 144));

        // Reset Buffer for new frame
        g_netSendBuffer.clear();

        // TODO: Recv Network Data
        net.ThreadLock.lock();
        for (NetBuffer& buffer : g_packetQueue)
            world.handlePacket(buffer);
        g_packetQueue.purge();
        net.ThreadLock.unlock();

        InputStruct inputs;
        getInputs(inputs);

        // TODO: Add Inputs to Buffer
        BUFFER_HEADER(g_netSendBuffer, HeaderType::MOVE);
        for (int i = 0; i < 6; i++)
        {
            BUFFER_DATA(g_netSendBuffer, inputs.inputs[i]);
        }

        net.Send(g_netSendBuffer.buffer, NetBuffer::size);

        world.update();

        // Render Everything!
        world.renderFrame(window);
        window.display();
    }

	return 0;
}