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


// TODO: FIX PROBLEMS PLEASSEE
struct QueueElement
{
	Message msg;
	sockaddr_in address;
};

//DynamicArray<QueueElement> packetQueue;

static QueueElement packetQueue[16];
static int counter = 0;

void NetworkThread()
{
	while (true)
	{
		QueueElement element;

		if (Recv(element.msg, element.address))
			continue;
		
		ANetwork::threadLock.lock();

		QueueElement& ref = packetQueue[counter];
		ref.msg.Copy(element.msg);
		ref.msg.bytes = element.msg.bytes;
		ref.address = element.address;

		counter++;

		//QueueElement& newElm = packetQueue.pushBack();

		//newElm.msg.Copy(element.msg);
		//newElm.msg.bytes = element.msg.bytes;
		//newElm.address = element.address;

		ANetwork::threadLock.unlock();
	}
}