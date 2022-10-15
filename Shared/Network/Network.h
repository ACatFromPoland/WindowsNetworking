#pragma once

#include "NetworkTypes.h"
#include "NetCollideable.h"
#include "NetClock.h"
#include "NetEntity.h"
#include "NetPlayer.h"

static NetBuffer g_netRecvBuffer;
static NetBuffer g_netSendBuffer;

typedef unsigned char i8;
typedef unsigned short i16;
typedef int i32;
typedef long long i64;

typedef float f32;

// BUFFER TOOLS
#define BUFFER_STARTER(buf) buf.write((i32)0);
#define BUFFER_HEADER(buf, var) buf.write(var); \
											(int*)buf.buffer[0]++;

#define BUFFER_DATA(buf, var) buf.write(var);
#define BUFFER_READ(buf, var) var = buf.read<decltype(var)>();