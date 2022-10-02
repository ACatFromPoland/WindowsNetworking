#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "DynamicArray.h"

typedef unsigned char byte;

// Who put this here?
//
inline void* myalloc(size_t size)
{
	return malloc(size);
}
#define malloc myalloc