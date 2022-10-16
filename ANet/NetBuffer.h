#pragma once

class NetBuffer
{
public:
	const static int size = 400;

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
	void write(T value)
	{
		*(T*)iter = value;
		iter += sizeof(T);
	}

	void pushBack(unsigned char* buffer, size_t size)
	{
		for (int i = 0; i < size; i++)
		{
			write<unsigned char>(buffer[i]);
		}
	}
};