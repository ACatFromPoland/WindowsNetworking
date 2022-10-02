#pragma once
#include "Utils.h"
#include "DynamicArray.h"

class Message : public DynamicArray<byte>
{
public:
	byte* iter;

	Message() : DynamicArray()
	{
		iter = nullptr;
	}

	Message(size_t t) : DynamicArray(t)
	{
		iter = nullptr;
	}

	void beginFetch()
	{
		iter = memory;
	}

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