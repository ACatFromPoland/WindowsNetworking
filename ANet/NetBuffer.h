#pragma once

class NetBuffer
{
public:
	const static int size = 1400;

	unsigned char buffer[size];
	unsigned char* iter;

	NetBuffer()
	{
		clear();
	}

	void clear()
	{
		iter = buffer;
	}

	template <class T>
	T read()
	{
		T* ptr = (T*)iter;
		iter += sizeof(T);
		return *ptr;
	}

	template <class T>
	void write(T value)
	{
		*(T*)iter = value;
		iter += sizeof(T);
	}
};
