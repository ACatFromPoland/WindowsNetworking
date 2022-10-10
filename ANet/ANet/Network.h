#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#include <mutex>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512

//#define RET_ERROR(x) return WSAError(x)

class Network
{
public:
	~Network()
	{
		CleanUp();
	}

	bool Setup();

	int WSAError(const char* Str);

	virtual void CleanUp();
};