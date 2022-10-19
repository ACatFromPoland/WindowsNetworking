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
	T& write(T value)
	{
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
};