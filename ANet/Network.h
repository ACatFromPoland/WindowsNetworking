#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#include <mutex>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

class Network
{
public:
	std::mutex ThreadLock;

	bool Setup();

	int WSAError(const char* Str);

	virtual void CleanUp() = 0;
};
