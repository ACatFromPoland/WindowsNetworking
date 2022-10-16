#include"NetBuffer.h"

NetBuffer::NetBuffer()
{
	clear();
}

void NetBuffer::clear()
{
	iter = buffer;
}

bool NetBuffer::isEmpty()
{
	return (iter == buffer);
}