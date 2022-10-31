#pragma once
#include "Vector2.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef float f32;
typedef double f64;

struct bitInt
{
	i32 value;

	void set(i32 v, bool b)
	{
		if (b) value |= v;
		else value &= ~v;
	}
	i32 get(i32 v)
	{
		return value & v;
	}
	void toggle(i32 v)
	{
		value ^= v;
	}

	void setAt(i32 i, bool b)
	{
		if (b) value |= (1 << i);
		else value &= ~(1 << i);
	}
	bool getAt(i32 i)
	{
		return (value & (1 << i));
	}
	void toggleAt(i32 i)
	{
		value ^= (1 << i);
	}
};