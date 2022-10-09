#include "ANet/Client.h"

#define RETERR(x) return WSAError(x)

int WSAError(const char* Str)
{
	printf("%s", Str);

	char buf[256];
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)buf, sizeof(buf), NULL);

	printf(" %ld: %s\n", WSAGetLastError(), buf);

	return 1;
}

int main()
{
	Client Net;
	Net.Setup();

	Net.Begin("127.0.0.1", 5006);

	const char* sendbuf = "this is a test";

	if (!Net.Send(sendbuf, 15))
		RETERR("Couldn't send data!");

	if (!Net.Recv())
		RETERR("CLIENT Error at recv!");
	
	system("pause");
	return 0;
}