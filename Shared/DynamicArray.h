#pragma once
//#include <stdlib.h>
#include <utility>
#include <memory>

inline void* myalloc(size_t size)
{
	return malloc(size);
}
#define malloc myalloc

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
			memory = (T*)realloc(memory, memorySize * sizeof(T));
		}
	}

	T& back()
	{
		return memory[count - 1];
	}

	// Thank you source engine :D
	inline T* construct(T& memory)
	{
		return new(&memory) T();
	}

	inline T* copyConstruct(T& memory, const T& src)
	{
		return new(&memory) T(src);
	}
	//
	
	T& pushBack()
	{
		count++;
		tryExpandMem();
		T& ref = memory[count - 1];
		construct(ref);
		return ref;
	}

	void pushBack(const T& element)
	{
		++count;
		tryExpandMem();
		copyConstruct(memory[count - 1], element);
	}

	void pushBack(const T* element, size_t size)
	{
		count += size;
		tryExpandMem();
		for (size_t i = 0; i < size; i++)
			copyConstruct(memory[count - size], element[i]);
	}

	void fastRemove(size_t index)
	{
		if (count - 1 != index)
		{
			memory[index] = memory[count - 1];
		}
		popBack();
	}

	void clear()
	{
		count = 0;
	}

	bool isEmpty()
	{
		return count == 0;
	}

	void purge()
	{
		for (int i = 0; i < count; i++)
			memory[i].~T();

		count = 0;
		memorySize = 0;
		free(memory);
		memory = nullptr;
	}

	void allocate(int _count)
	{
		memorySize = _count;
		memory = (T*)realloc(memory, memorySize * sizeof(T));
	}

	// Ops overloads
	T& operator[](size_t index) { return memory[index]; }

	DynamicArray<T>& operator=(const DynamicArray<T>& toCopy)
	{
		count = toCopy.count;
		memorySize = toCopy.memorySize;
		memory = (T*)malloc(toCopy.memorySize * sizeof(T));
		memcpy(memory, toCopy.memory, memorySize * sizeof(T));
		return *this;
	}

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