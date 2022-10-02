#include <SFML/Graphics.hpp>
#include "ClientNet.h"
#include <iostream>

void GetConnectionInfo(sockaddr_in& serverAddr);


int main()
{
	ANetwork::Setup();
	ANetwork::Bind();

	Message toSend;
	sockaddr_in serverAddr;
	GetConnectionInfo(serverAddr);

	Message toRecv;
	sockaddr_in serverAuth;

	// TODO: Thread Connection Process (Currently waits forever)
	// ... Make First Contact With Server
	FormatMessageData(toSend);

	ConnectPacket connect{};
	std::string name;
	std::cin >> name;
	strncpy_s(connect.name, name.c_str(), sizeof(connect.name));

	AddPacket<ConnectPacket>(toSend, PacketType::CLIENT_CONNECT, connect);

	Send(toSend, serverAddr);


	// ... Wait For Server Response
	if (Recv(toRecv, serverAuth))
	{
		printf("Response Failed!\n");
		system("pause");
		return 0;
	}
	HandleMessage(toRecv);



	// ... Wait for Server Data
	if (Recv(toRecv, serverAuth))
	{
		printf("Connection Failed!\n");
		system("pause");
		return 0;
	}
	HandleMessage(toRecv);



	// ... Send Response to Server Data
	FormatMessageData(toSend);

	ResponsePacket response{};
	AddPacket<ResponsePacket>(toSend, PacketType::ECHO_RESPOND, response);

	Send(toSend, serverAddr);



	// ... Now Connected, Start Game Loop
	std::thread networkThread(NetworkThread);
	NetClock clock(66.0f);
	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
	// SFML GRAPHICS
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color(146, 146, 144));


		// ... Wait for World/Server Updates
		ANetwork::threadLock.lock();
		if (ready)
		{
			HandleMessage(msgBuffer);
			ready = false;
		}
		/*
		for (int i = packetQueue.count - 1; i >= 0; i--)
		{
			QueueElement& q = packetQueue[i];
			HandleMessage(q.msg);
			packetQueue.popBack();
		}
		*/
		ANetwork::threadLock.unlock();


		if (clock.Tick())
		{
			// ... Send Player Data(Inputs...)
			FormatMessageData(toSend);

			InputPacket inputs;
			inputs.ID = client.id;

			inputs.input[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
			inputs.input[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
			inputs.input[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
			inputs.input[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

			AddPacket<InputPacket>(toSend, PacketType::CLIENT_INPUT, inputs);

			Send(toSend, serverAddr);
		}
		
		// TODO: Render Player
		

		window.display();
	}

	ANetwork::Close();
	return 0;
}

void GetConnectionInfo(sockaddr_in& serverAddr)
{
	printf("Options: 0 = Self Host, 1 = Outbound Host\n");
	int option;
	std::cin >> option;

	if (option)
	{
		printf("Enter Ip..\n");
		std::string ip;
		std::cin >> ip;
		ANetwork::FormatAddress(serverAddr, ip.c_str(), 5006);
	}
	else
		ANetwork::FormatAddress(serverAddr, "127.0.0.1", 5006);
}
