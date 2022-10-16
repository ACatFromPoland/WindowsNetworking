#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#include <mutex>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

class NetBase
{
public:
	std::mutex threadLock;

	// Sets up WSA
	bool Setup();

	// Call an Error
	int WSAError(const char* Str);

	// Template for cleaning up WSA
	virtual void CleanUp() = 0;
};
