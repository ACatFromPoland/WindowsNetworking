#pragma once
#include "Utils.h"

#include <WS2tcpip.h>
#include <mutex>
#include <thread>
#pragma comment (lib, "ws2_32.lib")

#define PACKET_MAX_SIZE 140

// Declaring because static

class ANetwork
{
public:
	static int ID;
	static SOCKET sock;
	static sockaddr_in local;
	static std::mutex threadLock;

	// Setups the Windows Socket API
	static void Setup();

	// Format char* and u_short to sockaddr_in
	static void FormatAddress(sockaddr_in& address, const char* ip, u_short port);

	// Format sockaddr_in to char*
	static void GetIPAddress(sockaddr_in& address, char* ip);

	// Binds the socket to a port
	static void Bind(u_short port = 0);

	// Sends a byte buffer to address
	static int Send(sockaddr_in& address, byte* data, size_t size);

	// Recvs a byte buffer and gets address
	static byte* Recv(sockaddr_in& address, int& bytesRecieved);

	// Closes the Windows Socket API
	static void Close();
};