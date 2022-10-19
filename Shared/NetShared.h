#pragma once
#include "Vector2.h"

typedef unsigned char i8;
typedef unsigned short i16;
typedef int i32;
typedef long long i64;

typedef float f32;

struct bitInt
{
	i32 value;

	void set(int v, bool b)
	{
		if (b) value |= v;
		else value &= ~v;
	}
	i32 get(int v)
	{
		return value & v;
	}
	void toggle(int v)
	{
		value ^= v;
	}

	void setAt(int i, bool b)
	{
		if (b) value |= (1 << i);
		else value &= ~(1 << i);
	}
	bool getAt(int i)
	{
		return (value & (1 << i));
	}
	void toggleAt(int i)
	{
		value ^= (1 << i);
	}
};