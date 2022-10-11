#pragma once
#include "Network.h"
#include "NetBuffer.h"
#include "NetObject.h"

// TODO: MAKE OTHER VIRTUAL FUNCTIONS PURE!
class NetVarBase
{
public:
	virtual void read(NetBuffer& buffer) = 0;
	virtual void write(NetBuffer& buffer) = 0;
};

template <class T>
class NetVar : public NetVarBase
{
public:
	T value;

	NetVar(DynamicArray<NetVarBase*>& variables)
	{
		// No value
		variables.pushBack(this);
	}

	virtual void read(NetBuffer& buffer)
	{
		value = buffer.read<T>();
	}
	virtual void write(NetBuffer& buffer)
	{
		buffer.write<T>(value);
	}

	// TODO: MORE OPERATORS

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

#define NetInt(name) NetVar<int> name = {netVariables}
#define NetFloat(name) NetVar<float> name = {netVariables}
#define NetBool(name) NetVar<bool> name = {netVariables}
#define NetVector2(name) NetVar<Vector2> name = {netVariables}
#define NetByte(name) NetVar<unsigned char> name = {netVariables}
#define NetEntType(name) NetVar<int> name = {netVariables}