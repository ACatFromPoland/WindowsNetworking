#pragma once
#include "Utils.h"

size_t roundUpToMultipleOfTwo(size_t v);

template<class T>
class DynamicArray {
public:
	size_t count;
	T* memory;

	DynamicArray()
	{
		count = 0;
		memorySize = 0;
		memory = nullptr;
	}

	DynamicArray(size_t size)
	{
		setSize(size);
	}

	~DynamicArray()
	{
		purge();
	}

	void copy(DynamicArray& dynamic1)
	{
		count = dynamic1.count;
		memorySize = dynamic1.memorySize;
		memory = (T*)malloc(dynamic1.memorySize * sizeof(T));
		memcpy(memory, dynamic1.memory, memorySize * sizeof(T));
	}

	void setSize(size_t c)
	{
		count = c;
		memorySize = roundUpToMultipleOfTwo(count);
		memory = (T*)malloc(memorySize * sizeof(T));
	}

	void popBack()
	{
		memory[count - 1].~T();
		count--;
	}

	void tryExpandMem()
	{
		if (count > memorySize)
		{
			memorySize = roundUpToMultipleOfTwo(count);
			T* temp = (T*)realloc(memory, memorySize * sizeof(T));
			memory = temp;
		}
	}

	T& pushBack()
	{
		count++;
		tryExpandMem();
		return memory[count - 1];
	}

	void pushBack(const T& element)
	{
		++count;
		tryExpandMem();
		memory[count - 1] = element;
	}

	void pushBack(const T* element, size_t size)
	{
		count += size;
		tryExpandMem();
		memcpy(&memory[count - size], element, size);
	}

	void fastRemove(size_t index)
	{
		T lastElement = memory[count - 1];
		memory[index] = lastElement;
		popBack();
	}

	void clear()
	{
		count = 0;
	}

	void purge()
	{
		for (int i = 0; i < count; i++)
			memory[i].~T();
		memory = nullptr;
		memorySize = 0;
		count = 0;
	}

	// Ops overloads
	T& operator[](size_t index) { return memory[index]; }
	T* begin() { return memory; }
	T* end() { return memory + count; }

private:
	size_t memorySize;
};

inline size_t roundUpToMultipleOfTwo(size_t v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	v++;

	return v;
}