#pragma once

template <class T>
T Read(unsigned char** buffer)
{
	// [in] ptr -> buffer
	unsigned char* p = (*buffer);
	// p == buffer
	(*buffer) += sizeof(T);

	return *(T*)(*buffer);
}

template <class T>
void Write(unsigned char** buffer)
{
	(*buffer) = *(unsigned char*)&T;
	(*buffer) += sizeof(T);
}
