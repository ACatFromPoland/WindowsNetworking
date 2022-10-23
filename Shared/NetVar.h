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

	NetVar(DynamicArray<ptrdiff_t>& netVars, ptrdiff_t diff)
	{
		netVars.pushBack(diff);
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
#define netOffset(s,m) ((::size_t)&reinterpret_cast<char const volatile&>((((s)0)->m)))
#define netConstructor(name) name = {netVars, netOffset(decltype(this), name)}

#define NetInt(name) NetVar<int> netConstructor(name)
#define NetFloat(name) NetVar<float> netConstructor(name)
#define NetBool(name) NetVar<bool> netConstructor(name)
#define NetByte(name) NetVar<unsigned char> netConstructor(name)
#define NetVector2(name) NetVar<Vector2> netConstructor(name)
//#define NetEntType(name) NetVar<int> name = {netVars, this}