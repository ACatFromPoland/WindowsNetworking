#pragma once
#include "Utils.h"
#include "DynamicArray.h"

class Message : public DynamicArray<byte>
{
public:
	byte* iter;
	int bytes;

	Message() : DynamicArray()
	{
		iter = nullptr;
		bytes = 0;
	}

	Message(size_t t) : DynamicArray(t)
	{
		iter = nullptr;
		bytes = 0;
	}

	void beginFetch()
	{
		iter = memory;
	}

	// templating
	template <class T>
	T* fetch()
	{
		T* temp = (T*)iter;
		iter += sizeof(T);

		return temp;
	}

	byte* fetch(size_t size)
	{
		byte* temp = iter;
		iter += size;

		return temp;
	}
};