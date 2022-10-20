#pragma once
#include <assert.h>

class NetBuffer
{
public:
	const static int size = 2048;

	unsigned char buffer[size];
	unsigned char* iter;
	
	NetBuffer();

	void clear();

	bool isEmpty();

	template <class T>
	T read()
	{
		T* ptr = (T*)iter;
		iter += sizeof(T);
		return (*ptr);
	}

	template <class T>
	T& write(T value)
	{
		assert(("NetBuffer overflow!\n", getBytesLeft() >= sizeof(T)));

		T* ptr = (T*)iter;
		*ptr = value;
		iter += sizeof(T);
		return *ptr;
	}

	template <class T>
	void write(T* buffer, int size)
	{
		for (int i = 0; i < size; i++)
			write<T>(buffer[i]);
	}

private:
	int getBytesLeft()
	{
		return size - (int)(iter - buffer);
	}
};