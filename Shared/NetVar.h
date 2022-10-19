#pragma once
#include "NetBuffer.h"
#include "NetObject.h"
#include "Vector2.h"

// TODO: MAKE OTHER VIRTUAL FUNCTIONS PURE!
class NetVarBase
{
public:
	virtual void read(NetBuffer& buffer) = 0;
	virtual void write(NetBuffer& buffer) = 0;
	virtual int getSize() = 0;
};

template <class T>
class NetVar : public NetVarBase
{
public:
	T value;

	NetVar(DynamicArray<ptrdiff_t>& netVars, void* parent)
	{
		netVars.pushBack((ptrdiff_t)this - (ptrdiff_t)parent);
	}

	virtual void read(NetBuffer& buffer)
	{
		value = buffer.read<T>();
	}

	virtual void write(NetBuffer& buffer)
	{
		buffer.write<T>(value);
	}

	virtual int getSize()
	{
		return sizeof(T);
	}

	// TODO: MORE OPERATORS

	T operator+(T v)
	{
		return T(value + v);
	}
		
	T operator-(T v)
	{
		return T(value - v);
	}
		
	T operator*(T v) 
	{
		return T(value * v);
	}
	T operator/(T v) 
	{
		return T(value / v);
	}

	void operator+=(T v)
	{
		value += v;
	}

	void operator-=(T v)
	{
		value -= v;
	}
	
	void operator=(T v)
	{
		value = v;
	}

	operator T()
	{
		return value;
	}
};

// netVariables exists in NetObject.h
// 
// #name would have made it into a string
#define NetInt(name) NetVar<int> name = {netVars, this}
#define NetFloat(name) NetVar<float> name = {netVars, this}
#define NetBool(name) NetVar<bool> name = {netVars, this}
#define NetByte(name) NetVar<unsigned char> name = {netVars, this}
#define NetVector2(name) NetVar<Vector2> name = {netVars, this}
//#define NetEntType(name) NetVar<int> name = {netVars, this}