#pragma once
#include "ANET.h"
#include "Message.h"
#include "NetStruct.h"
#include "Clock.h"

// 
// Network.h is a interface for the ANET, Message and NetStruct Headers
//

void Send(Message& msg, sockaddr_in& address)
{
	ANetwork::Send(address, msg.memory, msg.count);
}

// Replace AddressData with windows.h addr_in types
// NOW
bool Recv(Message& msg, sockaddr_in& address)
{
	int bytes;

	byte* buf = ANetwork::Recv(address, bytes);
	if (!buf)
	{
		return true;
	}
	msg.clear();
	msg.pushBack(buf, bytes);
	msg.bytes = bytes;

	free(buf);
	return false;
}

void FormatMessageData(Message& msg)
{
	StarterPacket starter = { ANetwork::ID++, 0 };

	msg.clear();
	msg.pushBack((byte*)&starter, sizeof(starter));
}

template <class T>
void AddPacket(Message& msg, PacketType type, T& data)
{
	HeaderPacket header = { type };
	msg.pushBack((byte*)&header, sizeof(header));
	msg.pushBack((byte*)&data, sizeof(T));
}


struct QueueElement
{
	Message msg;
	sockaddr_in address;
};

//DynamicArray<QueueElement> packetQueue;
//int packetsRecieved;
Message msgBuffer;
sockaddr_in addrBuffer;
bool ready;
void NetworkThread()
{
	ready = false;

	while (true)
	{
		if (Recv(msgBuffer, addrBuffer))
			continue;
		ANetwork::threadLock.lock();
		ready = true;
		ANetwork::threadLock.unlock();
	}
	/*
	packetsRecieved = 0;

	while (true)
	{
		QueueElement element;
		Recv(element.msg, element.address);
		packetsRecieved++;
		printf("%llu - %llu / %llu     %d\n", element.msg.count, packetQueue.count, packetsRecieved, element.msg.memory[0]);

		Network::threadLock.lock();
		
		QueueElement& newElm = packetQueue.pushBack();
		newElm.msg.Copy(element.msg);
		newElm.address = element.address;

		Network::threadLock.unlock();
	}
	*/
}